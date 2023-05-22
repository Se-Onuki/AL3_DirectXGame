#pragma once

#include "DirectXCommon.h"
#include "WinApp.h"
#include <imgui.h>

class ImGuiManager {
public:
	static ImGuiManager* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// DirectX基盤インスタンス（借りてくる）
	DirectXCommon* dxCommon_ = nullptr;
	// SRV用ヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator=(const ImGuiManager&) = delete;
};
