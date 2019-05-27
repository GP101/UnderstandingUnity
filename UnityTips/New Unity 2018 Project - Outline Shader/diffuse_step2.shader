Shader "Custom/Diffuse"
{
    Properties
    {
	    _MainTex ("Main Texture", 2D) = "white" {}
	    _Color ("Main Color", Color) = (1,1,1,1)
		_OutlineColor ("Outline Color", Color) = (1,0,0,1)
		_Outline ("Outline width", Range (0.0, 0.03)) = .01
    }

	CGINCLUDE
	    #include "UnityCG.cginc"
		struct appdata {
			float4 vertex : POSITION;
			float3 normal : NORMAL;
			float2 texcoord : TEXCOORD0;
		};

		struct v2f {
			float4 pos : POSITION;
			float3 normal : NORMAL;
			float2 texcoord : TEXCOORD0;
		};
	    float4 _LightColor0;
	    sampler2D _MainTex;
	    float4 _Color;
	    uniform float _Outline;
	    uniform float4 _OutlineColor;
	ENDCG

    SubShader
    {
		Pass {
			Cull Off
			ZWrite Off
			ZTest Always
			Lighting Off
 
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
             
            v2f vert(appdata v) {
                // just make a copy of incoming vertex data but scaled according to normal direction
                v2f o;
                o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
     
                float3 norm   = mul ((float3x3)UNITY_MATRIX_IT_MV, v.normal);
                float2 offset = TransformViewToProjection(norm.xy);
     
                o.pos.xy += offset * o.pos.z * _Outline;
                o.normal = v.normal;
                o.texcoord = v.texcoord;
                return o;
            }

            half4 frag(v2f i) :COLOR {
                return float4(_OutlineColor.rgb,1);
            }
            ENDCG
		}

	    Pass
	    {  
		    Tags { "LightMode" = "ForwardBase" }
			ZWrite On
			ZTest LEqual
			Lighting On

		    CGPROGRAM
			    #pragma vertex vert
			    #pragma fragment frag

			    v2f vert(appdata v)
			    {
				    v2f o;
				    o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				    o.normal = mul(_Object2World,float4(v.normal, 0.0)).xyz;
				    o.texcoord = v.texcoord;
				    return o;
			    }
			
			    float4 frag(v2f v) : COLOR
			    {
				    float4 texColor = tex2D(_MainTex, v.texcoord);

				    float3 normalDirection = normalize(v.normal);
         		    float3 lightDirection = normalize(_WorldSpaceLightPos0.xyz);
         		    float3 diffuse = _LightColor0.rgb * _Color.rgb * max(0.0, dot(normalDirection, lightDirection));

				    return float4(diffuse,1) * texColor;
			    }
		    ENDCG
	    }//Pass
    }//SubShader

    Fallback "Diffuse"
}//Shader
