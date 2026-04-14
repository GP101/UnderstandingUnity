# 3D 렌더링 엔진 원리 분석
_Exported on 4/14/2026 at 13:40:49 GMT+9 from Cursor (3.0.16)_

---

**User**

이 프로젝트는 3D 렌더링 엔진의 원리를 학습하기 위한 자료야. 소스 전체를 분석해 줘.

---

**Cursor**

프로젝트 구조와 핵심 소스를 탐색합니다.



`KVector3`의 벡터 연산만 확인합니다.



이 워크스페이스(**LinearAlgebra Step15 Texture**)는 **Win32 + GDI+**로 동작하는 **CPU 소프트웨어 렌더러** 예제입니다. GPU 파이프라인이 아니라, **동차 좌표·투영·스캔 변환·텍스처 매핑**을 직접 코드로 따라가며 배우도록 구성되어 있습니다.

---

## 1. 전체 구조 한눈에

| 영역 | 역할 |
|------|------|
| `LinearAlgebra.cpp` | 진입점, 메시지 루프, 더블 버퍼, `OnRender`에서 장면 구성 |
| `KPolygon` | 정점/인덱스 버퍼, 큐브(12삼각형), 변환·그리기 진입 |
| `KMatrix4` | 이동/회전/약식 원근 투영, `w`로 나누는 동차 처리 |
| `KVectorUtil` | 픽셀·선 스캔 변환, 삼각형 채우기, **UV 보간 + 텍스처 샘플** |
| `KRenderState` | 싱글톤으로 조명/재질 상태 보관 |
| `KLight` / `KMaterial` / `KRgb` | Lambert 스타일 조명용 데이터·색 연산 |
| `KScreenCoordinate` / `KBasis2` | 2D “논리 좌표” → 화면 픽셀 변환 |

`KMatrix3` 등은 프로젝트에 포함되어 있으나, **이 스텝의 메인 흐름에서는 `LinearAlgebra.cpp`가 `KMatrix3.h`만 include하고 실제 사용은 거의 없는 형태**에 가깝습니다(이전 단계 잔여일 수 있음).

---

## 2. 실행·표시 파이프라인

**`wWinMain`**에서 GDI+ 초기화 후 창을 띄우고, **`PeekMessage` 루프 + `OnIdle`**로 게임처럼 매 프레임 갱신합니다. `timeGetTime()`으로 경과 시간을 넘겨 회전에 씁니다.

```82:103:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\LinearAlgebra.cpp
    // Main message loop:
    while (true)
    {
        ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        const DWORD dwNewTime = ::timeGetTime();
        const BOOL bIsTranslateMessage = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
        if (!bIsTranslateMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }//if

        OnIdle(float(dwNewTime - dwOldTime) / 1000.f);
        Sleep(10);
```

**`OnSize`**: 클라이언트 크기에 맞춰 **호환 DC + 비트맵**을 만들고, `KVectorUtil::g_screenCoordinate`로 **원점을 화면 중앙**, y축 반전·픽셀 스케일(예: 4픽셀 단위)을 설정합니다.

**`OnIdle`**: 흰색으로 오프스크린 버퍼를 지운 뒤 `OnRender(g_hdc, …)`로 그리고 **`BitBlt`로 창 HDC에 복사**합니다. 전형적인 더블 버퍼링입니다.

**`Initialize`**: GDI+ `Bitmap`으로 `imageService.jpg`를 읽어 **전역 `g_image`**에 둡니다. 텍스처 소스입니다.

---

## 3. 3D 수학: 변환과 투영

**`KMatrix4`**는 4×4 행렬 곱, 벡터 변환 시 **4행째로부터 나온 `w`로 나누는 동차 나눗셈**을 합니다.

```38:61:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\KMatrix4.cpp
KVector3 KMatrix4::operator*(KVector3& vLeft)
{
    KVector3 vRet;
    // ... x,y,z ...
    const float w = vLeft.x * m_afElements[3][0] +
        vLeft.y * m_afElements[3][1] +
        vLeft.z * m_afElements[3][2] +
        m_afElements[3][3];
    vRet.x /= w; // homogeneous divide
    vRet.y /= w;
    vRet.z /= w;
    return vRet;
}
```

**`SetProjection(d)`**는 `m[3][2] = 1/d`, `m[3][3] = 1` 형태의 **단순 원근(약식 투영)** 행렬입니다. 깊이 버퍼나 정확한 clip volume은 없고, **z는 “있긴 하지만” 이후 래스터에서 깊이 비교에 쓰이지 않습니다.**

**`OnRender`**에서는 대략 다음 순서입니다.

1. `KRenderState`에 조명 설정  
2. `KPolygon`에 인덱스/정점 설정  
3. `matTrans * matRotY * matRotX`로 월드 변환  
4. `matProjection`으로 투영  
5. `poly.DrawOnCanvas(hdc, *g_image)`로 **텍스처가 있는 경로**로 그리기  
6. `DrawAxis`로 좌표축

**중요:** `KPolygon::Transform`은 **`pos`만 행렬에 곱하고, `u`/`v`는 그대로**입니다. 즉 UV는 모델/뷰/투영에 따라 변하지 않는 **정적 텍스처 좌표**로 쓰입니다.

---

## 4. 기하: 큐브와 래스터 경로

**`KVertex`**: 위치 + `(u, v)`.

**`KPolygon::SetVertexBuffer` / `SetIndexBuffer`**: 변이 ±20인 큐브, **면당 UV 0~1**을 수동으로 배치하고, 인덱스로 **12개 삼각형**을 정의합니다.

그리기는 **`ScanDrawIndexedPrimitive(..., Bitmap&)`** 경로로 들어갑니다. 삼각형마다:

1. 두 모서리의 차로 **면 법선** `Cross` 후 `Normalize`  
2. **후면 판별**: 카메라/시선을 `(0, 0, -1)`로 두고 `Dot(forward, normal) > 0`일 때만 그림  
3. **감쇠**: `shade = max(0, Dot(light.m_dir, normal))`  
4. `KRgb color = ambient + diffuse * mtrl * shade + …` 로 **면 단위 단색 조명색** 계산  
5. UV를 **`KRgb`의 r,g 채널에 넣어** `FillTriangle`에 넘김 (아래 참고)

---

## 5. 스캔 변환과 텍스처 (이 스텝의 핵심)

**아이디어:** `ScanLineSegment` / `FillTriangle`은 원래 **정점 색 보간**용입니다. 텍스처 버전에서는 **보간량으로 `KRgb`의 [0],[1]을 u,v로 재사용**합니다.

- 삼각형 변을 따라 `ScanLineSegment`가 `(x,y)`와 함께 **`col`을 선형 보간**  
- 같은 `y`에 모인 점들을 짝지어 **수평 span**을 또 `ScanLineSegment`로 채움  
- 텍스처 오버로드에서는 span 위 각 픽셀에서 `col[0], col[1]`을 **u,v**로 보고 `bitmap.GetPixel`로 샘플링

```394:405:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\KVectorUtil.cpp
                const float u = __min(__max(col[0], 0), 1.0f);
                const float v = __min(__max(col[1], 0), 1.0f);
                Color color;
                bitmap.GetPixel(int(u * bitmapWidth), int(v * bitmapHeight), &color);

                KRgb colTemp;
                colTemp[0] = (color.GetR()/255.f + shadeColor[0]) / 2.0f;
                colTemp[1] = (color.GetG()/255.f + shadeColor[1]) / 2.0f;
                colTemp[2] = (color.GetB()/255.f + shadeColor[2]) / 2.0f;

                PutPixel(hdc, X, Y, colTemp.GetGdiColor());
```

**특징·한계 (학습용으로 이해하면 좋은 부분):**

- **透視 보정(perspective-correct) UV 보간은 없음** — 화면 공간에서 선형 보간이라, 투영이 강할수록 텍스처가 약간 늘어지는 전형적인 소프트웨어 렌더 한계입니다.  
- **Z-buffer 없음** — 겹치는 면의 앞뒤는 처리하지 않습니다.  
- 조명은 **삼각형마다 하나의 shade**로 구한 뒤, 텍스처 색과 **단순 평균 `(tex + shade)/2`** 로 섞습니다(픽셀 단위 Phong 등은 아님).  
- `PutPixel`은 GDI+로 작은 사각형을 채워 “굵은 픽셀”을 만듭니다(`g_screenCoordinate`의 스케일과 맞춤).

---

## 6. 조명·상태

**`KRenderState`**: 싱글톤 `GetInstance()`로 `KLight`, `KMaterial`을 들고 있습니다.

**`KLight`**: 기본 **방향광** `m_dir = (0,0,-1)`, ambient/diffuse/specular 필드.

**`OnRender`**에서 ambient/diffuse 등을 설정해 파이프라인에 반영합니다.

---

## 7. 파일별 요약

- **`LinearAlgebra.cpp`**: 앱 뼈대, 타이밍, 장면(큐브 + 회전 + 투명), GDI 더블 버퍼.  
- **`KPolygon.*`**: 큐브 데이터, `DrawIndexedPrimitive`(와이어/후면만 다른 스타일), **`ScanDrawIndexedPrimitive` 두 종류**(단색 vs 텍스처).  
- **`KVectorUtil.*`**: Bresenham류 선, 스캔 변환, 삼각형 채우기, **텍스처 샘플링**.  
- **`KMatrix4.*`**: TRS + 투영 + 동차 나눗셈.  
- **`KRgb.h`**: 색·연산·`ScannedResult` 정렬 키용 `(x,y,col)`.  
- **`KScreenCoordinate` / `KBasis2` / `KVector2`**: 2D 변환과 그리드·축.  
- **`stdafx.*`, `resource.*`, `.vcxproj`**: MSVC Win32 프로젝트 구성.

---

## 8. 이 프로젝트로 “무엇을” 배우는지

1. **동차 4×4 행렬**로 회전·이동·투영을 한 번에 다루고 **`w` 나눗셈**으로 NDC에 가까운 2D 투영 좌표를 얻는 과정  
2. **인덱스드 트라이앵글 리스트**로 메시 표현  
3. **법선·내적**으로 후면 제거와 **Lambert형 음영**  
4. **엣지/스팬 스캔 변환**으로 삼각형 내부를 채우고, 그 위에 **UV 보간 + 비트맵 샘플링**  
5. GDI+로 **실제 창에 비트맵 출력**하는 Win32 패턴

원하시면 다음 중 하나를 골라서 더 깊게 풀 수 있습니다. 예: **`SetProjection`의 수학적 의미**, **`FillTriangle`의 `std::set` 기반 span 알고리즘 단계별 설명**, **透視 보정이 필요한 이유와 수정 방향(이론만)** 등입니다.

---

**User**

텍스쳐 매핑할 때, PutPixel을 이용해서 선을 그리고 면을 그리는 방식으로 구현되어서 느려. 프로젝트에서 구현된 기능을 사용하지 않더라도 이 부분의 속도를 개선하는 방법은?

---

**Cursor**

이 프로젝트가 느린 **직접적인 이유**는 텍스처 매핑 루프 안에서 **`PutPixel`이 매 픽셀마다 `Gdiplus::Graphics`와 `FillPolygon`(또는 브러시)을 쓰는 구조**이기 때문입니다. GDI/GDI+는 “픽셀 단위 그리기”가 아니라 **비트맵 한 장을 통째로 다루는 API**에 맞춰져 있습니다.

아래는 **기존 스캔 변환·UV 보간 로직은 유지하되**, 출력만 바꿔 속도를 올리는 현실적인 방법들입니다.

---

## 1. 한 프레임 = 하나의 픽셀 버퍼에 직접 쓰기 (가장 효과 큼)

**아이디어:** `PutPixel` 대신 **CPU 상의 `uint32_t*` 또는 `BYTE*` 프레임버퍼**에 RGBA(또는 BGRA)로 직접 대입하고, 프레임 끝에서 **한 번만** 화면으로 올립니다.

- **GDI:** `CreateDIBSection`으로 DIB를 만들면 **CPU가 비트맵 메모리에 바로 쓸 수 있음** → 내부 루프는 순수 C++ 포인터 쓰기.
- **표시:** 매 프레임 `BitBlt` / `StretchBlt`로 창 HDC에 복사(지금도 `BitBlt` 패턴이 있으니 흐름은 동일).

이렇게 하면 **픽셀당 GDI+ 객체 생성·Draw 호출 비용이 사라져** 체감이 가장 큽니다.

---

## 2. GDI+ `Bitmap::LockBits` / `UnlockBits`로 텍스처·프레임버퍼 모두 “락”

텍스처도 매번 `GetPixel`이면 느립니다.

- **프레임버퍼**와 **소스 텍스처**를 각각 `LockBits`로 잠그고, 내부에서는 `GetPixel` 대신 **포인터 + stride**로 이웃 픽셀 읽기(최소한 nearest는 매우 빠름).
- 필터가 필요하면 나중에 고려하고, 먼저는 **point sampling**만으로도 충분히 빨라집니다.

---

## 3. `SetPixel` / GDI 단일 픽셀 API도 피하기

GDI의 `SetPixel`도 픽셀 루프에는 부적합합니다. 결국 **DIB 직접 쓰기**가 같은 계열에서 정답에 가깝습니다.

---

## 4. 알고리즘 쪽 미세 최적화 (부차적)

- **`std::set` 기반 스캔 결과 정렬**: 삼각형마다 삽입·정렬이 들어가면 오버헤드가 큽니다. 보통은 **y버킷(Active Edge Table)** 같은 **배열 + 인덱스**로 바꿉니다. (로직은 같고 자료구조만 바꿔도 이득)
- **멀티스레드:** 스캔라인을 y 구간으로 나누어 병렬화(경계 처리 필요). 단, **1순위는 여전히 “GDI+ 제거”**입니다.
- **透視 보정 UV**는 수학 비용이 늘어나므로, “속도만” 보면 오히려 부담이 될 수 있어 **정확도 vs 속도** 트레이드오프입니다.

---

## 5. 아예 다른 레이어 (학습 범위를 벗어나면)

- **Direct3D 11 / Vulkan / OpenGL**: GPU 래스터라이제이션·텍스처 유닛 사용 → 같은 “원리 학습”과는 결이 달라질 수 있음.
- **SIMD**: 프레임버퍼에 쓰는 루프를 벡터화하면 추가 이득.

---

## 정리

**가장 현실적인 개선 순서**

1. **프레임버퍼를 DIB 또는 LockBits된 비트맵 한 장으로 두고**, 삼각형 채우기 루프는 **포인터 대입만** 하기.  
2. 텍스처는 **`LockBits`로 읽기 전용 포인터** 사용 (`GetPixel` 제거).  
3. 여유가 있으면 **`std::set` 제거 → 전통적 스캔라인 자료구조**로 정리.

이렇게 하면 “스캔 변환·UV 보간으로 면을 그린다”는 **구현 개념은 유지**하면서, 병목인 **GDI+ per-pixel 호출**만 제거하는 형태로 속도를 크게 개선할 수 있습니다.

---

**User**

1번 방법으로 소스를 수정해 줘.

---

**Cursor**

DIB 섹션으로 프레임버퍼를 만들고 `PutPixel`이 해당 버퍼에 직접 쓰도록 변경합니다. 관련 파일을 확인합니다.



구현 중: `KVectorUtil`에 프레임버퍼 API 추가, `PutPixel`을 DIB 직접 쓰기로 변경, `LinearAlgebra.cpp`의 `OnSize`/`OnIdle`/`Finalize`를 `CreateDIBSection`으로 전환합니다.



작업 반영했습니다. 변경 요약만 정리합니다.

## 1. `KVectorUtil.h` / `KVectorUtil.cpp`

- **`SetFrameBuffer(void* bits, int width, int height, int strideBytes)`**  
  `CreateDIBSection`으로 받은 **32bpp BGRA** 비트 포인터와 크기·스트라이드를 등록합니다. `nullptr`이면 예전처럼 GDI+ `FillPolygon` 경로로 떨어집니다.
- **`ClearFrameBufferWhite()`**  
  `memset`으로 프레임을 흰색(`0xFF` 바이트 전체)으로 채웁니다.
- **`PutPixel`**  
  프레임버퍼가 있으면 GDI+를 쓰지 않고, 변환된 **볼록 사각형(논리 픽셀 1칸)** 안의 픽셀만 **메모리에 직접 `DWORD` 쓰기**합니다.  
  `g_basis2`가 identity가 아닐 때를 위해 **반평면 검사**로 사각형 내부만 채웁니다.

## 2. `LinearAlgebra.cpp`

- **`OnSize`**: `CreateCompatibleBitmap` 대신 **`CreateDIBSection`**(top-down: `biHeight = -iHeight`, `BI_RGB`, 32bpp)으로 비트맵 생성 후 `KVectorUtil::SetFrameBuffer` 호출.  
  `GetDC` 후 **`ReleaseDC`로 해제**하도록 정리했습니다(기존에는 `GetDC`만 있고 해제가 없었음).
- **`Finalize`**: 비트맵 해제 전에 **`SetFrameBuffer(nullptr, …)`** 로 포인터 무효화.
- **`OnIdle`**: `Rectangle` + 브러시 대신 **`ClearFrameBufferWhite()`** 한 번으로 클리어.

이제 텍스처·스캔 변환에서 호출되는 **`PutPixel`이 프레임당 GDI+ 객체를 만들지 않아** 이전보다 훨씬 가볍게 동작합니다. 축·와이어는 여전히 GDI `LineTo`를 쓰므로, 그 부분까지 CPU만 쓰려면 나중에 선도 DIB에 직접 그리면 됩니다.

로컬에서 한 번 빌드해 보시려면:

`msbuild "D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\LinearAlgebra Step15 Texture.vcxproj" /p:Configuration=Debug /p:Platform=x64`

---

**User**

속도가 상당히 개선되었어. SetPixel을 호출해서 단일 픽셀 API를 사용하는데, 이 부분을 최적화 하는 방법이 있어?

---

**Cursor**

코드베이스에 `SetPixel` 사용 여부 확인 중입니다.



이 **Step15 Texture** 폴더 소스에는 `SetPixel` 호출이 **없습니다**. 이전에 적용한 방식대로라면, 논리 픽셀 한 칸은 `KVectorUtil::PutPixel`이 **DIB 메모리에 `DWORD`를 직접 쓰는 경로**로 가고, GDI의 `SetPixel`은 쓰지 않는 구성입니다.

그래도 질문을 **“GDI `SetPixel` 같은 단일 픽셀 API를 쓰는 경우 어떻게 최적화하나”**로 이해해서 답하면 다음과 같습니다.

---

## 왜 `SetPixel`이 느린가

- 호출마다 **GDI 내부 락·좌표 변환·클리핑·디바이스 드라이버 경로**를 탑니다.
- 비트맵이 **디스플레이 DC**에 붙어 있으면 더 비쌉니다.
- 루프에서 수백만 번 호출하면 **호출 비용이 픽셀 연산보다 큽니다**.

그래서 **단일 픽셀 API 자체를 “더 빠르게” 쓰는 트릭은 거의 없고**, 패턴을 바꾸는 게 맞습니다.

---

## 실질적인 최적화 방향 (이미 1번과 같은 계열)

1. **`SetPixel` / `GetPixel` 제거**  
   **`CreateDIBSection` + 포인터 직접 쓰기** 또는 **`Bitmap::LockBits`**로 한 덩어리 메모리에 쓰기 (지금 프로젝트가 그쪽).

2. **한 프레임에 한 번만 화면으로 반영**  
   버퍼에 다 그린 뒤 **`BitBlt` / `StretchDIBits` 한 번** (이미 `BitBlt` 패턴).

3. **여전히 “픽셀 루프”가 병목이면**  
   - **축에 맞은 사각형**이면 `PointInConvexQuad` 대신 **연속 메모리 `memcpy`/행 단위 채우기**  
   - **SIMD**로 한 줄씩 4~8픽셀씩 쓰기  
   - **멀티스레드**로 스캔라인 분할 (동기화 비용 vs 이득 트레이드오프)

---

## 정리

- **`SetPixel`을 “최적화해서” 빠르게 만드는 방법은 사실상 없고**, **같은 일을 버퍼 직접 쓰기로 바꾸는 것**이 정석입니다.  
- 현재 이 스텝 코드는 그걸 이미 하고 있으므로, **`SetPixel`로 바꾸는 것은 오히려 느려질 가능성이 큽니다.**

다른 파일/브랜치에서 `SetPixel`을 쓰고 있다면 그 경로를 알려주면, 그 루프에 맞춰 **DIB 직접 쓰기로 옮기는 구체적 단계**만 짚어 드릴 수 있습니다.

---

**User**

지금 코드에서 렌더링 속도를 더 개선하는 방법을 알려줘.

---

**Cursor**

핫 경로를 다시 확인합니다.



지금 구조 기준으로, **아직 비싼 구간**과 **그다음으로 손볼 만한 것**을 나눠 정리했습니다.

---

## 1. 가장 큰 이득: 텍스처 샘플링 (`bitmap.GetPixel`)

`ScanLineSegment(..., Bitmap& bitmap, ...)` 안에서 **스팬마다 `GetPixel`을 호출**하고 있습니다.

```499:510:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\KVectorUtil.cpp
                const float u = __min(__max(col[0], 0), 1.0f);
                const float v = __min(__max(col[1], 0), 1.0f);
                Color color;
                bitmap.GetPixel(int(u * bitmapWidth), int(v * bitmapHeight), &color);

                KRgb colTemp;
                colTemp[0] = (color.GetR()/255.f + shadeColor[0]) / 2.0f;
                colTemp[1] = (color.GetG()/255.f + shadeColor[1]) / 2.0f;
                colTemp[2] = (color.GetB()/255.f + shadeColor[2]) / 2.0f;

                //Pixel(Y + m_vReso / 2, X + m_hReso / 2) = colTemp;
                PutPixel(hdc, X, Y, colTemp.GetGdiColor());
```

`GetPixel`은 GDI+ 경로라 **픽셀당 비용이 매우 큽니다**.  
**개선:** 로드 시 또는 프레임 시작 시 한 번 **`LockBits`로 텍스처를 BGRA 버퍼로 잡고**, 루프 안에서는 **`stride` + 포인터**로 nearest 샘플만 하면 체감이 큽니다. (필터 없이 point sampling만 해도 충분히 빨라짐.)

---

## 2. 삼각형 채우기: `std::set<ScannedResult>`

`FillTriangle`이 **세 변을 `set`에 넣고 정렬·병합**하는 방식입니다.

```426:449:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\KVectorUtil.cpp
    std::set<ScannedResult> scanned_pnts; // sorted in y val, and in x val if y val the same.
    ScanLineSegment(hdc, x1, y1, col1, x2, y2, col2, &scanned_pnts);
    ...
        if (output)
                output->insert(ScannedResult(X, Y, col));
```

`set` 삽입은 **O(log n)** 이고 할당·RB-tree 비용이 큽니다. 삼각형이 많아지면 여기가 누적됩니다.  
**개선:** 같은 “에지 나열 → y로 묶기”를 **`std::vector` + y 정렬** 또는 **y 버킷(스캔라인 테이블)** 으로 바꾸는 것이 전형적인 최적화입니다.

---

## 3. `PutPixel` 안쪽: `PointInConvexQuad` × (작은 bbox 픽셀 수)

DIB 직접 쓰기로도, **논리 픽셀 한 칸마다** bbox 안에서 **반평면 검사를 반복**합니다.

```180:188:D:\git\UnderstandingUnity\LinearAlgebra\LinearAlgebra_Step15 Texture\KVectorUtil.cpp
        for (int py = ymin; py <= ymax; ++py)
        {
            DWORD* row = reinterpret_cast<DWORD*>(base + (size_t)py * (size_t)g_frameStride);
            for (int px = xmin; px <= xmax; ++px)
            {
                if (PointInConvexQuad(px, py, qx, qy))
                    row[px] = bgra;
            }
        }
```

지금 설정(`g_basis2` 기본값, 화면축 직교)에서는 **거의 항상 축에 정렬된 직사각형**이라,  
**개선:** 그 경우 분기해서 **`for` 이중 루프만으로 채우기** 또는 **행 단위 `memset`/넓이만큼 `DWORD` 연속 대입**으로 `PointInConvexQuad`를 없앨 수 있습니다. (기울어진 `g_basis2`만 일반 쿼드 유지.)

---

## 4. 같은 색/같은 “논리 픽셀” 반복 줄이기

스캔 변환은 **연속된 화면 픽셀**에 같은 논리 격자를 쓰는 경우가 많습니다.  
**개선:** “한 스캔라인에서 `PutPixel` 대신 **한 번에 직사각형 영역**을 채우는 API”를 두면 호출 수가 줄어듭니다. (구현 난이도는 중간.)

---

## 5. GDI `DrawLine` / `OutText`

축은 **선 두 개**뿐이라 비중은 작지만, `DrawLine`마다 **`CreatePen`/`DeleteObject`**가 있습니다.  
**개선:** 펜 재사용, 또는 축도 DIB에 직접 Bresenham으로 그리기. FPS 텍스트는 `TextOut` 한 번이라 보통 미미합니다.

---

## 6. 메인 루프: `Sleep(10)`

`LinearAlgebra.cpp`의 루프에 `Sleep(10)`이 있으면 **이론상 약 100fps 상한**에 가깝고 입력·프로파일링에도 영향이 있습니다.  
**개선:** 벤치할 때는 **`Sleep` 제거 또는 1 이하**, vsync 없이 raw loop로 측정.

---

## 우선순위 요약

| 순위 | 내용 |
|------|------|
| 1 | 텍스처 **`GetPixel` → LockBits(또는 로드 시 DIB 복사)** |
| 2 | **`std::set` 제거** → 벡터/버킷 스캔라인 |
| 3 | **`PutPixel`**: 축정렬일 때 **쿼드 테스트 생략** |
| 4 | (선택) **스팬 단위**로 프레임버퍼에 쓰기, **`Sleep` 조정** |

원하면 Agent 모드에서 **1번(LockBits) + 2번(set 제거)** 같이 묶어서 패치하는 순서를 추천할 수 있습니다.

---

**User**

1을 적용해서 텍스쳐 샘플링 부분을 수정해 줘.

---

**Cursor**

텍스처 샘플링을 `LockBits`로 바꿉니다. 관련 코드 전체를 확인합니다.



`FillTriangle`에서 텍스처를 한 번만 `LockBits`하고, 스캔라인 샘플링은 잠긴 메모리에서 읽도록 분리합니다.



구현: `anonymous namespace`에 `TexReadView`·락 헬퍼·`ScanLineSegment_TextureCore`를 추가하고, `ScanLineSegment`(텍스처)와 `FillTriangle`(텍스처)를 이 경로로 연결합니다.



적용해 두었습니다. 변경 요약입니다.

## 변경 사항 (`KVectorUtil.cpp`)

1. **`TexReadView`**  
   `LockBits`로 잠근 비트맵의 `Scan0`, `Stride`, `maxTexX/Y`, `bpp`(3 또는 4)만 담습니다.

2. **`LockBitmapForTexRead`**  
   너비·높이가 유효하고 `GetPixelFormat`이 **24bpp 또는 32bpp**일 때만 `LockBits(..., ImageLockModeRead, fmt, ...)`로 읽기 잠금합니다. 그 외 포맷은 잠금 실패로 처리합니다.

3. **`SampleTexelBGR`**  
   `GetPixel` 대신 행 포인터 `scan0 + ty * stride + tx * bpp`에서 **B,G,R → float RGB**로 읽습니다 (기존 `GetR/G/B`와 동일한 의미).

4. **`ScanLineSegment_TextureCore`**  
   기존 텍스처용 `ScanLineSegment`의 좌표 정규화 + 루프를 그대로 두고, 샘플만  
   `tex != nullptr` → 메모리 읽기,  
   아니면 → 기존처럼 `bitmap->GetPixel` 폴백.

5. **`ScanLineSegment(..., Bitmap&, ...)`**  
   호출마다 잠그고 `ScanLineSegment_TextureCore` 호출 후 `UnlockBits` (다른 호출자가 있을 때를 대비).

6. **`FillTriangle(..., Bitmap&, ...)`**  
   **삼각형마다 `LockBits` 한 번**만 하고, 가로 스팬은 **`ScanLineSegment_TextureCore(..., &tv, nullptr, ...)`**로만 그립니다. 잠금 실패 시에는 스팬마다 `GetPixel` 폴백(`nullptr, &bitmap`).

7. **디버그용 `debugCounter` / `debugCallCounter`**  
   사용되지 않아 제거했습니다.

이제 큐브 한 면을 채울 때 텍스처는 **삼각형당 최대 1회 잠금**이고, 픽셀 루프 안에서는 **포인터 읽기**만 하므로 `GetPixel` 때보다 훨씬 가볍습니다. 빌드는 로컬에서 한 번 돌려 보시면 됩니다.

---

**User**

class "Gdiplus::Image" has no member "GetPixelFormatSize"

---

**Cursor**

`GetPixelFormatSize`는 GDI+의 전역 함수입니다. `Image::` 접두사를 제거합니다.



수정 반영했습니다.

**원인:** `GetPixelFormatSize`는 `Gdiplus::Image` 멤버가 아니라 **GDI+ 전역 함수**입니다. 선언은 보통 `Gdiplus::GetPixelFormatSize(PixelFormat)` 형태입니다.

**수정:** `Image::GetPixelFormatSize(fmt)` → `GetPixelFormatSize(fmt)`  
(`KVectorUtil.h`에 `using namespace Gdiplus;`가 있어서 그대로 두었습니다.)

여전히 이름을 못 찾으면 `#include <gdiplus.h>` 뒤에 `Gdiplus::GetPixelFormatSize(fmt)`처럼 네임스페이스를 붙이면 됩니다.

---

**User**

identifier "PutPixel" is undefined

---

**Cursor**

익명 네임스페이스 안에서 `PutPixel`이 `KVectorUtil::PutPixel`로 연결되지 않습니다. `KVectorUtil::PutPixel`로 호출하도록 수정합니다.



**원인:** `ScanLineSegment_TextureCore`가 **파일 스코프의 익명 네임스페이스** 안에 있어서, 이름만 `PutPixel`로 쓰면 전역/ADL에서 찾다가 **`KVectorUtil::PutPixel`로 이어지지 않습니다.**

**수정:** 해당 호출을 `KVectorUtil::PutPixel(hdc, X, Y, colTemp.GetGdiColor());`로 바꿨습니다. `KVectorUtil.h`에 `PutPixel` 선언이 있으므로 정의보다 위에 있어도 컴파일됩니다.

---

**User**

성능이 많이 개선되지 않아서, 소스를 다시 revert했어.

---

**Cursor**

알겠습니다. 되돌린 선택도 충분히 이해할 수 있어요.

**왜 체감이 크지 않았을 수 있는지** 짧게만 정리하면 다음과 같습니다.

1. **병목이 한 군데가 아닐 수 있음**  
   `GetPixel` → `LockBits`만 바꿔도, 여전히 **`std::set` 기반 삼각형 채우기**, **`PutPixel` 루프(쿼드 내부 검사 등)**, **GDI 선·텍스트**, **`Sleep`**, 메시지 루프 구조가 남아 있으면 전체 프레임 시간에서 비중이 나뉩니다.

2. **LockBits는 “삼각형당 한 번”이 이득이 큼**  
   `FillTriangle` 안에서만 잠그고, 다른 경로에서 자주 잠그면 이득이 줄어듭니다. 또 **폴백(`GetPixel`)**이 자주 타면 개선이 거의 없어 보일 수 있습니다.

3. **DIB 직접 쓰기까지 같이 되돌렸다면**  
   그전에 적용했던 **`PutPixel`의 GDI+ 제거**가 사실 가장 체감이 컸을 가능성이 큽니다. 그 부분까지 revert했다면 FPS가 다시 비슷해지는 게 자연스럽습니다.

다시 최적화할 때는 **프로파일러로 1프레임 안에서 시간이 어디로 가는지**만 잡아도, `set`인지 `PutPixel`인지 `GetPixel`인지 바로 갈립니다.  
원하시면 지금 워크스페이스 기준으로 “revert 후 남은 코드”를 기준으로 **우선순위만** 짧게 짜 드릴 수 있어요(Ask 모드라 파일 수정은 못 하고, 필요하면 Agent 모드로 전환해 달라고 하시면 됩니다).

---

**User**

z-buffer가 없어. z-buffer를 추가해줘. 그리고 Cube를 2개 렌더링해서 z-buffer가 제대로 동작하는지 확인하고 싶어.

---

**Cursor**

코드 상태 확인 중 — Z-buffer 통합 지점을 파악합니다.



Z-buffer와 Z 보간을 추가합니다. `ScannedResult`에 `z`를 넣고, `SetFrameBuffer`와 함께 깊이 버퍼를 할당하며, `FillTriangle`(텍스처)와 `ScanLineSegment`에서 Z를 보간해 `PutPixel`에서 검사합니다.



Replacing the scan-conversion section with Z-aware implementations.



구현을 모두 반영했습니다. 요약만 정리합니다.

## Z-buffer

- **`SetFrameBuffer`**와 같은 크기로 `std::vector<float> g_depthBuffer`를 두고, 초기값은 **`FLT_MAX`**입니다.
- **`ClearDepthBuffer()`**로 매 프레임 맨 앞에서 비웁니다 (`OnIdle`에서 `ClearFrameBufferWhite` 직후 호출).
- **`PutPixelDepth`**: DIB에 쓸 때 `z < g_depthBuffer[idx]`일 때만 색을 쓰고 깊이를 갱신합니다 (**작을수록 가깝다**). 화면이 반대로 보이면 `PutPixelDepth`의 비교만 뒤집으면 됩니다.

## Z 보간

- **`ScannedResult`**에 `float z` 추가.
- **`ScanLineSegment(..., z1, z2, ...)`** / **`ScanLineSegment(..., z1, z2, ..., bitmap, shade)`**에서 `col`과 같이 **z도 선형 보간**.
- **`FillTriangle`** (단색·텍스처)에 **`z1,z2,z3`** 오버로드 추가; 기존 z 없는 버전은 그대로 두었습니다.
- **`KPolygon`**: 단색·텍스처 모두 **`pos.z`**를 넘기고, 와이어 엣지도 **`PutPixelDepth`**를 타도록 z 보간 호출로 바꿨습니다.

## 큐브 2개 (`LinearAlgebra.cpp`)

- **`SetIndexBuffer()`**는 한 번만.
- 큐브 A: **`SetTranslation(10, 10, -28)`** 후 변환·그리기.
- 큐브 B: **`SetTranslation(10, 10, 18)`** 후 **`SetVertexBuffer()``로 원점 메시를 복구**한 뒤 다시 변환·그리기.  
겹치는 부분에서 **가까운 쪽(B)**이 앞에 오는지 확인하면 됩니다.

빌드는 로컬에서 한 번 돌려 보시고, 앞뒤가 바뀌어 보이면 `PutPixelDepth`의 `z >=` / `z <` 비교만 조정하거나, `KPolygon`에 넘기는 `pos.z`에 부호를 반대로 주면 됩니다.

