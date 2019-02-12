#pragma once

typedef unsigned __int8		byte;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;

#define UNKNOW_INDEX_ELEMENTS		(int)(-1)
#define MAIN_DEVICE					"Main Device"
#define MAIN_CAMERA					"Main Camera"

// Name resource

#define NAME_BEHAVIOR				"Behavior "
#define NAME_INDEXES				" Indexes - "
#define NAME_MATERIAL				"Material "
#define NAME_MESH					"Mesh "
#define NAME_MESH_FILTER			"MeshFilter "
#define NAME_MESH_RENDER			"MeshRender "
#define NAME_PROPERTY				"property: "
#define NAME_TEXTURE_2D				"Texture 2D "
#define NAME_TRANSFORM				"Transform "
#define NAME_SHADER					"Shader "
#define NAME_VERTICES				" Vertices"

// Property block

#define PB_MATRIX_TRANSFORM			"main_position"
#define PB_MATRIX_VIEW_PROJECTION	"camera_view_projection"

//

#define NAME_DEFAULT				"default"

// G-Buffer

#define NAME_G_BUFFER_DIFFUSE		"G-Buffer Diffuse"
#define NAME_G_BUFFER_NORMAL		"G-Buffer Normal"
#define NAME_G_BUFFER_SPECULAR		"G-Buffer Specular"
#define NAME_G_BUFFER_DEPTH			"G-Buffer Depth"
