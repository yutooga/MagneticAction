﻿#pragma once
//===============================================
//
// プリコンパイル済みヘッダー
//  ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる。
//  全てのcppからインクルードされる必要がある。
//
//===============================================


//===============================================
//
// 基本
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <fileSystem>

//===============================================
//
// Direct3D11
//
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioWin8.lib")
#include <SimpleMath.h>
#include <Audio.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

//============================================
// imgui
//============================================
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_stdlib.h"

//===============================================
//
// Effekseer
//
//===============================================
#ifdef _DEBUG
#pragma comment(lib, "Effekseerd.lib")
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#else
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")
#endif

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include"Application/GameObject/Manager/PlayerManager/PlayerManager.h"

//===============================================
// 文字コード変換
//===============================================
#include <strconv.h>

//===============================================
// jsonファイル読み込み
//===============================================
#include"nlohmann/json.hpp"

//===============================================
//
// 自作Framework
//
//===============================================
#include "Framework/KdFramework.h"
