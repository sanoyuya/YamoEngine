#pragma once
#include <d3d12.h>
#include<wrl.h>
#include<memory>
#include<array>

enum class BlendMode
{
	None,//�m�[�u�����h
	Alpha,//����������
	Add,//���Z����
	Sub,//���Z����
	Mul,//��Z����
	Inv//�F���]
};

enum class ShaderMode
{
	Basic,//�W��
	Phong,//�t�H���V�F�[�f�B���O
	Toon,//�g�D�[���V�F�[�f�B���O
	RimLight,//�������C�g
	MultiPhong,//�}���`�����_�[�^�[�Q�b�g�p
	SimpleColor//�P�F�V�F�[�_�[�p
};

struct PipelineSet
{
	//���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//�p�C�v�����X�e�[�g�̐���
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

struct Blob
{
	//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>vs;
	//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>ps;
	//�W�I���g���V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob>gs;
};

class Pipeline
{
public://�X�v���C�g

	/// <summary>
	/// �X�v���C�g�p�p�C�v���C���̐���
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="blend">�u�����h���[�h</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreateSpritePipline(Blob& blob, BlendMode blend, ID3D12Device* device, std::array<PipelineSet, 6>& pip);

	/// <summary>
	/// �p�[�e�B�N���p�p�C�v���C���̐���
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="blend">�u�����h���[�h</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreateParticlePipline(Blob& blob, BlendMode blend, ID3D12Device* device, std::array<PipelineSet, 6>& pip);

public://���f��

	/// <summary>
	/// Basic���f���p�p�C�v���C������
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="blend">�u�����h���[�h</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreateBasicModelPipline(Blob& blob, BlendMode blend, ID3D12Device* device, std::array<PipelineSet, 6>& pip);

	/// <summary>
	/// Phong���f���p�p�C�v���C������
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="blend">�u�����h���[�h</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreatePhongModelPipline(Blob& blob, BlendMode blend, ID3D12Device* device, std::array<PipelineSet, 6>& pip);

	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�gPhong���f���p�p�C�v���C������
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="blend">�u�����h���[�h</param>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreateMultiPhongModelPipline(Blob& blob, BlendMode blend, ID3D12Device* device, std::array<PipelineSet, 6>& pip);
	
public://�|�X�g�G�t�F�N�g

	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�p�C�v���C������
	/// </summary>
	/// <param name="blob">�V�F�[�_�[�I�u�W�F�N�g</param>
	/// <param name="pip">�p�C�v���C��</param>
	static void CreatePostEffectPipline(Blob& blob, PipelineSet& pip);

	static void CreateMultiTexturePipline(Blob& blob, PipelineSet& pip);
};