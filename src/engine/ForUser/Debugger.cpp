#include "Debugger.h"
#include"FrameWork/Fps.h"
#include"KuroEngine.h"

std::vector<KuroEngine::Debugger*>KuroEngine::Debugger::s_debuggerArray;
int KuroEngine::Debugger::s_id = 0;
const std::string KuroEngine::Debugger::s_jsonFileDir = "resource/engine/";
const std::string KuroEngine::Debugger::s_jsonName = "KuroEngineDebugger";
const std::string KuroEngine::Debugger::s_jsonExt = ".json";
KuroEngine::JsonData KuroEngine::Debugger::s_parameterLog;

void KuroEngine::Debugger::Draw()
{
	ImGui::Begin("DebuggerMgr");
	Fps::Instance()->OnImguiItems();
	ImGui::Separator();
	for (auto& debugger : s_debuggerArray)
	{
		ImGui::Checkbox(debugger->m_title.c_str(), &debugger->m_active);
	}
	ImGui::End();

	for (auto& debugger : s_debuggerArray)
	{
		if (!debugger->m_active)continue;
		ImGui::Begin(debugger->m_title.c_str(), nullptr, debugger->m_imguiWinFlags);

		//カスタムパラメータウィンドウのアクティブ状態
		if (!debugger->m_customParamList.empty())
		{
			ImGui::Checkbox("CustomParameter", &debugger->m_customParamActive);
			ImGui::Separator();
		}

		//ユーザー定義のimgui処理
		debugger->OnImguiItems();

		ImGui::End();

		//カスタムパラメータウィンドウ
		if (debugger->m_customParamActive)
		{
			std::string winTitle = debugger->m_title + " - CustomParameter";
			ImGui::Begin(winTitle.c_str(), nullptr, debugger->m_imguiWinFlags);
			debugger->CustomParameterOnImgui();
			ImGui::End();
		}
	}
}

void KuroEngine::Debugger::LoadParameterLog()
{
	using namespace KuroEngine;

	//ログがない
	if (!s_parameterLog.m_jsonData.contains(m_title))return;

	for (auto& param : m_customParamList)
	{
		const auto* jsonObj = &s_parameterLog.m_jsonData[m_title];
		for (auto& key : param.m_key)
		{
			//キーが存在しない
			if (!jsonObj->contains(key))continue;

			//階層を下げる
			jsonObj = &jsonObj->at(key);
		}

		switch (param.m_type)
		{
		case PARAM_TYPE::INT:
		{
			auto data = jsonObj->get<int>();
			memcpy(param.m_dataPtr, &data, sizeof(int));
			break;
		}
		case PARAM_TYPE::INT_VEC2:
		{
			Vec2<int>data = { (int)jsonObj[0],(int)jsonObj[1] };
			memcpy(param.m_dataPtr, &data, sizeof(int) * 2);
			break;
		}
		case PARAM_TYPE::INT_VEC3:
		{
			Vec3<int>data = { (int)jsonObj[0],(int)jsonObj[1],(int)jsonObj[2] };
			memcpy(param.m_dataPtr, &data, sizeof(int) * 3);
			break;
		}
		case PARAM_TYPE::INT_VEC4:
		{
			Vec4<int>data = { (int)jsonObj[0],(int)jsonObj[1],(int)jsonObj[2],(int)jsonObj[3] };
			memcpy(param.m_dataPtr, &data, sizeof(int) * 4);
			break;
		}

		case PARAM_TYPE::FLOAT:
		{
			auto data = jsonObj->get<float>();
			memcpy(param.m_dataPtr, &data, sizeof(float));
			break;
		}
		case PARAM_TYPE::FLOAT_VEC2:
		{
			Vec2<float>data = { (float)jsonObj[0],(float)jsonObj[1] };
			memcpy(param.m_dataPtr, &data, sizeof(float) * 2);
			break;
		}
		case PARAM_TYPE::FLOAT_VEC3:
		{
			Vec3<float>data = { (float)jsonObj[0],(float)jsonObj[1],(float)jsonObj[2] };
			memcpy(param.m_dataPtr, &data, sizeof(float) * 3);
			break;
		}
		case PARAM_TYPE::FLOAT_VEC4:
		{
			Vec4<float>data = { (float)jsonObj[0],(float)jsonObj[1],(float)jsonObj[2],(float)jsonObj[3] };
			memcpy(param.m_dataPtr, &data, sizeof(float) * 4);
			break;
		}

		case PARAM_TYPE::BOOL:
		{
			bool data = jsonObj->get<bool>();
			memcpy(param.m_dataPtr, &data, sizeof(bool));
			break;
		}

		case PARAM_TYPE::STRING:
		{
			std::string data = jsonObj->get<std::string>();
			memcpy(param.m_dataPtr, &data, sizeof(data));
			break;
		}

		default:
			AppearMessageBox("Debugger：LoadParameterLog()失敗", param.m_label + "のタイプがNONEだったので上手く読み込めなかったよ");
			break;
		}
	}
}

void KuroEngine::Debugger::WriteParameterLog()
{
	using namespace KuroEngine;

	if (m_customParamList.empty())return;

	s_parameterLog.m_jsonData[m_title] = nlohmann::json::object();

	for (auto& param : m_customParamList)
	{
		nlohmann::json* parent = &s_parameterLog.m_jsonData[m_title];

		for (int keyIdx = 0; keyIdx < static_cast<int>(param.m_key.size() - 1); ++keyIdx)
		{
			if (!parent->contains(param.m_key[keyIdx]))
			{
				(*parent)[param.m_key[keyIdx]] = nlohmann::json::object();
			}
			parent = &(*parent)[param.m_key[keyIdx]];
		}

		nlohmann::json& paramObj = *parent;
		auto key = param.m_key.back();
		switch (param.m_type)
		{
		case PARAM_TYPE::INT:
			paramObj[key] = *(int*)param.m_dataPtr;
			break;
		case PARAM_TYPE::INT_VEC2:
			paramObj[key] = { ((int*)param.m_dataPtr)[0],((int*)param.m_dataPtr)[1] };
			break;
		case PARAM_TYPE::INT_VEC3:
			paramObj[key] = { ((int*)param.m_dataPtr)[0],((int*)param.m_dataPtr)[1],((int*)param.m_dataPtr)[2] };
			break;
		case PARAM_TYPE::INT_VEC4:
			paramObj[key] = { ((int*)param.m_dataPtr)[0],((int*)param.m_dataPtr)[1],((int*)param.m_dataPtr)[2],((int*)param.m_dataPtr)[3] };
			break;

		case PARAM_TYPE::FLOAT:
			paramObj[key] = *(float*)param.m_dataPtr;
			break;
		case PARAM_TYPE::FLOAT_VEC2:
			paramObj[key] = { ((float*)param.m_dataPtr)[0],((float*)param.m_dataPtr)[1] };
			break;
		case PARAM_TYPE::FLOAT_VEC3:
			paramObj[key] = { ((float*)param.m_dataPtr)[0],((float*)param.m_dataPtr)[1],((float*)param.m_dataPtr)[2] };
			break;
		case PARAM_TYPE::FLOAT_VEC4:
			paramObj[key] = { ((float*)param.m_dataPtr)[0],((float*)param.m_dataPtr)[1],((float*)param.m_dataPtr)[2],((float*)param.m_dataPtr)[3] };
			break;

		case PARAM_TYPE::BOOL:
			paramObj[key] = *(bool*)param.m_dataPtr;
			break;

		case PARAM_TYPE::STRING:
			paramObj[key] = *(std::string*)param.m_dataPtr;
			break;

		default:
			AppearMessageBox("Debugger：WriteParameterLog()失敗", param.m_label + "のタイプがNONEだったので上手く書き込めなかったよ");
			break;
		}
	}
}

void KuroEngine::Debugger::CustomParameterOnImgui()
{

	//登録されたカスタムパラメータの設定
	for (auto& paramGroup : m_customParamGroup)
	{
		ImGui::SetNextItemOpen(true);
		ImGui::PushItemWidth(100);

		if (ImGui::TreeNode("%s", paramGroup.first.c_str()))
		{
			auto& itemList = paramGroup.second;

			std::vector<CustomParameter*>::iterator itr = itemList.begin();
			for (; itr != itemList.end();)
			{
				const auto& item = **itr;
				const char* label = item.m_label.c_str();
				bool error = false;
				float min = item.m_isMinMax ? item.m_min : 0.0f;
				float max = item.m_isMinMax ? item.m_max : 0.0f;

				switch (item.m_type)
				{
				case PARAM_TYPE::INT:
					ImGui::DragInt(label, (int*)(item.m_dataPtr), m_customParamDragSpeed, (int)min, (int)max);
					break;
				case PARAM_TYPE::INT_VEC2:
					ImGui::DragInt2(label, (int*)(item.m_dataPtr), m_customParamDragSpeed, (int)min, (int)max);
					break;
				case PARAM_TYPE::INT_VEC3:
					ImGui::DragInt3(label, (int*)(item.m_dataPtr), m_customParamDragSpeed, (int)min, (int)max);
					break;
				case PARAM_TYPE::INT_VEC4:
					ImGui::DragInt4(label, (int*)(item.m_dataPtr), m_customParamDragSpeed, (int)min, (int)max);
					break;

				case PARAM_TYPE::FLOAT:
					ImGui::DragFloat(label, (float*)(item.m_dataPtr), m_customParamDragSpeed, min, max);
					break;
				case PARAM_TYPE::FLOAT_VEC2:
					ImGui::DragFloat2(label, (float*)(item.m_dataPtr), m_customParamDragSpeed, min, max);
					break;
				case PARAM_TYPE::FLOAT_VEC3:
					ImGui::DragFloat3(label, (float*)(item.m_dataPtr), m_customParamDragSpeed, min, max);
					break;
				case PARAM_TYPE::FLOAT_VEC4:
					ImGui::DragFloat4(label, (float*)(item.m_dataPtr), m_customParamDragSpeed, min, max);
					break;

				case PARAM_TYPE::BOOL:
					ImGui::Checkbox(label, (bool*)item.m_dataPtr);
					break;

				case PARAM_TYPE::STRING:
				{
					std::string* strPtr = (std::string*)item.m_dataPtr;
					ImGui::InputText(label, (char*)strPtr->c_str(), strPtr->capacity() + 1);
					break;
				}

				default:
					AppearMessageBox("Debugger：CustomParamterOnImgui()失敗", item.m_label + "のタイプがNONEだったのでimgui上でいじれないよ");
					error = true;
					break;
				}

				if (error)itr = itemList.erase(itr);
				else itr++;
			}

			ImGui::TreePop();
		}
	}

}
