#include "DrawExcuteIndirect.h"
#include"DirectX12/D3D12App.h"

DrawExcuteIndirect::DrawExcuteIndirect(const InitDrawIndexedExcuteIndirect &INIT_DATA) :initData(INIT_DATA)
{
	drawType = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	//コマンドシグネチャ---------------------------
	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = INIT_DATA.argument.data();
	desc.NumArgumentDescs = static_cast<UINT>(INIT_DATA.argument.size());
	desc.ByteStride = sizeof(DrawIndexedIndirectCommand);

	HRESULT lResult =
		KuroEngine::D3D12App::Instance()->GetDevice()->CreateCommandSignature(
			&desc,
			INIT_DATA.rootsignature.Get(),
			IID_PPV_ARGS(&commandSig)
		);
	//コマンドシグネチャ---------------------------
	if (lResult != S_OK)
	{
		assert(0);
	}

	//Indirect用のバッファ生成
	m_cmdBuffer = KuroEngine::D3D12App::Instance()->GenerateRWStructuredBuffer(sizeof(DrawIndexedIndirectCommand), 1);

	DrawIndexedIndirectCommand command;
	command.drawArguments.IndexCountPerInstance = INIT_DATA.indexNum;
	command.drawArguments.InstanceCount = INIT_DATA.elementNum;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;
	command.view = INIT_DATA.updateView;
	m_cmdBuffer->Mapping(&command);
}

DrawExcuteIndirect::DrawExcuteIndirect(const InitDrawExcuteIndirect &INIT_DATA)
{
	drawType = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	//コマンドシグネチャ---------------------------
	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = INIT_DATA.argument.data();
	desc.NumArgumentDescs = static_cast<UINT>(INIT_DATA.argument.size());
	desc.ByteStride = sizeof(DrawIndirectCommand);

	HRESULT lResult =
		KuroEngine::D3D12App::Instance()->GetDevice()->CreateCommandSignature(
			&desc,
			INIT_DATA.rootsignature.Get(),
			IID_PPV_ARGS(&commandSig)
		);
	//コマンドシグネチャ---------------------------
	if (lResult != S_OK)
	{
		assert(0);
	}


	//Indirect用のバッファ生成

	m_cmdBuffer = KuroEngine::D3D12App::Instance()->GenerateRWStructuredBuffer(sizeof(DrawIndirectCommand), 1);

	DrawIndirectCommand command;
	command.drawArguments.VertexCountPerInstance = INIT_DATA.vertNum;
	command.drawArguments.InstanceCount = INIT_DATA.elementNum;
	command.drawArguments.StartVertexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.view = INIT_DATA.updateView;
	m_cmdBuffer->Mapping(&command);
}

void DrawExcuteIndirect::Draw(KuroEngine::GraphicsPipeline &pipeline,const Microsoft::WRL::ComPtr<ID3D12Resource> &COUNTER_BUFFER)
{
	pipeline.SetPipeline(KuroEngine::D3D12App::Instance()->GetCmdList());
	KuroEngine::D3D12App::Instance()->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画情報
	switch (drawType)
	{
	case D3D12_INDIRECT_ARGUMENT_TYPE_DRAW:
		initData.particleVertex->SetView(KuroEngine::D3D12App::Instance()->GetCmdList());
		break;
	case D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED:
		initData.particleVertex->SetView(KuroEngine::D3D12App::Instance()->GetCmdList());
		initData.particleIndex->SetView(KuroEngine::D3D12App::Instance()->GetCmdList());
		break;
	default:
		break;
	}

	D3D12_RESOURCE_BARRIER barrier1 =
		CD3DX12_RESOURCE_BARRIER::Transition(
			m_cmdBuffer->GetResource()->GetBuff().Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
		);
	KuroEngine::D3D12App::Instance()->GetCmdList()->ResourceBarrier(1, &barrier1);

	KuroEngine::D3D12App::Instance()->GetCmdList()->ExecuteIndirect
	(
		commandSig.Get(),
		1,
		m_cmdBuffer->GetResource()->GetBuff().Get(),
		0,
		COUNTER_BUFFER.Get(),
		0
	);

	D3D12_RESOURCE_BARRIER barrier2 =
		CD3DX12_RESOURCE_BARRIER::Transition(
			m_cmdBuffer->GetResource()->GetBuff().Get(),
			D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		);
	KuroEngine::D3D12App::Instance()->GetCmdList()->ResourceBarrier(1, &barrier2);

}
