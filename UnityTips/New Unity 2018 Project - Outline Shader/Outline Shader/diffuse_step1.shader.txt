Shader "Custom/Diffuse"
{
    Properties
    {
	    _MainTex ("Main Texture", 2D) = "white" {}
	    _Color ("Main Color", Color) = (1,1,1,1)
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
	ENDCG

    SubShader
    {
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
