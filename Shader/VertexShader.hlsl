struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 uv : TEXTURE0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 posw : WORLD_POS;
	float4 nor : NORMAL;
	float4 uv : TEXTURE0;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	float4		CameraPos;			// �J�������W
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[
	float4		LightPosMain;
	float4		LightPos1;
	float4		LightPos2;
	float		light_amount;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	// ���[�J�����W * ���[���h���W�ϊ��s��
	output.pos = mul(input.pos, World);
	// ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, View);
	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, Projection);

	// ���[���h���W��n��
	output.posw = mul(World, input.pos).xyz;

	// Texture�w��
	output.uv = input.uv;

	float4 normal;
	// �ړ����v�Z�ɔ��f�����Ȃ�
	input.nor.w = 0.0;
	// ���_�̖@���Ƀ��[���h�s����|�����킹��
	// ���[���h���W��ł̖@���̌����ɕϊ�����
	normal = mul(input.nor, World).xyzw;
	normal = normalize(normal);
	// saturate => �����Ŏw�肵���l��0�`1�Ԃł͈̔͂Ɏ��߂�
	// dot => ���όv�Z
	output.nor = normal;

	return output;
}
