#include "SkyBox.hlsli"

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	float32_t4x4 WorldViewProjection = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
	output.position = mul(input.position, mul(gTransformationMatrix.matWorld, WorldViewProjection)).xyww;
	output.texcoord = input.position.xyz;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverseTranspose));
	output.worldPosition = mul(input.position, gTransformationMatrix.matWorld).xyz;
	return output;
}