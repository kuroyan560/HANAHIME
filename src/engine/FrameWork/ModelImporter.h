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

	class ModelImporter
	{
	private:
		static ModelImporter* s_instance;

	public:
		static ModelImporter* Instance()
		{
			assert(s_instance != nullptr);
			return s_instance;
		}

	private:

		//モデル専用頂点
		using Vertex = KuroEngine::ModelMesh::Vertex;

		//エラーメッセージ表示
		void ErrorMessage(const std::string& FuncName, const bool& Fail, const std::string& Comment);
		//HSMロードで用いるデータの読み取り関数（成功すればtrueを返す)
		bool LoadData(FILE* Fp, void* Data, const size_t& Size, const int& ElementNum);
		//HSMセーブで用いるデータの書き込み関数（成功すればtrueを返す)
		bool SaveData(FILE* Fp, const void* Data, const size_t& Size, const int& ElementNum);

#pragma region glTF関連
		void LoadGLTFPrimitive(ModelMesh& ModelMesh, const Microsoft::glTF::MeshPrimitive& GLTFPrimitive, const Microsoft::glTF::Skin* GLTFSkin, const Microsoft::glTF::GLTFResourceReader& Reader, const Microsoft::glTF::Document& Doc);
		void PrintDocumentInfo(const Microsoft::glTF::Document& document);
		void PrintResourceInfo(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resourceReader);
		void LoadGLTFMaterial(const MATERIAL_TEX_TYPE& Type, std::weak_ptr<Material>AttachMaterial, const Microsoft::glTF::Image& Img, const std::string& Dir, const Microsoft::glTF::GLTFResourceReader& Reader, const Microsoft::glTF::Document& Doc);
#pragma endregion

		//インポートしたモデル
		std::map<std::string, std::shared_ptr<Model>>m_models;
		void RegisterImportModel(const std::string& Dir, const std::string& FileName, const std::shared_ptr<Model>& Model)
		{
			m_models[Dir + FileName] = Model;
		}
		std::shared_ptr<Model> CheckAlreadyExsit(const std::string& Dir, const std::string& FileName);

		//HSMが存在する場合をHSMから読み込めるようにするか(HSMのフォーマットが変わったときはフラグをオフにする)
		const bool m_canLoadHSM = true;

		//※ ファイル名は拡張子つき
		std::shared_ptr<Model> LoadGLTFModel(const std::string& Dir, const std::string& FileName, const std::string& Ext);
	public:
		ModelImporter()
		{
			assert(s_instance == nullptr);
			s_instance = this;
		}
		~ModelImporter() {  }

		std::shared_ptr<Model>LoadModel(const std::string& Dir, const std::string& FileName);
	};
}