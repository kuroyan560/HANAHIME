#pragma once
#include"Common/Singleton.h"
#include<string>
#include<vector>
#include<memory>
#include<map>
#include<forward_list>
#include"Render/RenderObject/Animation.h"
#include"Render/RenderObject/ModelInfo/ModelMesh.h"
#include"Render/RenderObject/ModelInfo/Skeleton.h"

#include<GLTFSDK/GLTF.h>
#include"StreamReader.h"

namespace KuroEngine
{
	class Model;

	class Importer
	{
	private:
		static Importer* s_instance;

	public:
		static Importer* Instance()
		{
			assert(s_instance != nullptr);
			return s_instance;
		}

	private:

		//���f����p���_
		using Vertex = KuroEngine::ModelMesh::Vertex;

		//�G���[���b�Z�[�W�\��
		void ErrorMessage(const std::string& FuncName, const bool& Fail, const std::string& Comment);
		//HSM���[�h�ŗp����f�[�^�̓ǂݎ��֐��i���������true��Ԃ�)
		bool LoadData(FILE* Fp, void* Data, const size_t& Size, const int& ElementNum);
		//HSM�Z�[�u�ŗp����f�[�^�̏������݊֐��i���������true��Ԃ�)
		bool SaveData(FILE* Fp, const void* Data, const size_t& Size, const int& ElementNum);

#pragma region glTF�֘A
		void LoadGLTFPrimitive(ModelMesh& ModelMesh, const Microsoft::glTF::MeshPrimitive& GLTFPrimitive, const Microsoft::glTF::Skin* GLTFSkin, const Microsoft::glTF::GLTFResourceReader& Reader, const Microsoft::glTF::Document& Doc);
		void PrintDocumentInfo(const Microsoft::glTF::Document& document);
		void PrintResourceInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader);
		void LoadGLTFMaterial(const MATERIAL_TEX_TYPE& Type, std::weak_ptr<Material>AttachMaterial, const Microsoft::glTF::Image& Img, const std::string& Dir, const Microsoft::glTF::GLTFResourceReader& Reader, const Microsoft::glTF::Document& Doc);
#pragma endregion

		//�C���|�[�g�������f��
		std::map<std::string, std::shared_ptr<Model>>m_models;
		void RegisterImportModel(const std::string& Dir, const std::string& FileName, const std::shared_ptr<Model>& Model)
		{
			m_models[Dir + FileName] = Model;
		}
		std::shared_ptr<Model> CheckAlreadyExsit(const std::string& Dir, const std::string& FileName);

		//HSM�����݂���ꍇ��HSM����ǂݍ��߂�悤�ɂ��邩(HSM�̃t�H�[�}�b�g���ς�����Ƃ��̓t���O���I�t�ɂ���)
		const bool m_canLoadHSM = true;

		//�� �t�@�C�����͊g���q��
		std::shared_ptr<Model> LoadGLTFModel(const std::string& Dir, const std::string& FileName, const std::string& Ext);
	public:
		Importer()
		{
			assert(s_instance == nullptr);
			s_instance = this;
		}
		~Importer() {  }

		std::shared_ptr<Model>LoadModel(const std::string& Dir, const std::string& FileName);
	};
}