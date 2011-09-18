-- see also http://www.xbdev.net/3dformats/x/x_custom_parser/index_01.php

-- EASY-TO-REMEMBER-ID-NAME ID-NUMBER STRING-VALUE
local X_TEMPLATE = 0xFF	 -- template
local X_UNKNOWN = 0xFE -- I added this so that we could identify unknown templates
local X_ERROR = 0x0
local X_HEADER	= 0x1 -- Header
local X_VECTOR = 0x2 -- Vector
local X_COORDS2D = 0x3 -- Coords2d
local X_QUATERNION	= 0x4 -- Quaternion
local X_MATRIX4X4 = 0x5 -- Matrix4x4
local X_COLORRGBA = 0x6 -- ColorRGBA
local X_COLORRGB = 0x7 -- ColorRGB
local X_INDEXEDCOLOR = 0x8 -- IndexedColor
local X_BOOLEAN	 = 0x9 -- Boolean
local X_BOOLEAN2D = 0x10 -- Boolean2d
local X_MATERIAL = 0x11 -- Material
local X_TEXTUREFILENAME	 = 0x12 -- TextureFilename
local X_MESHFACE = 0x13 -- MeshFace
local X_MESHFACEWRAPS = 0x14 -- MeshFaceWraps
local X_MESHTEXTURECOORDS = 0x15 -- MeshTextureCoords
local X_MESHNORMALS	 = 0x16 -- MeshNormals
local X_MESHVERTEXCOLORS = 0x17 -- MeshVertexColors
local X_MESHMATERIALLIST = 0x18 -- MeshMaterialList
local X_MESH = 0x19 -- Mesh
local X_FRAMETRANSFORMMATRIX = 0x20 -- FrameTransformMatrix
local X_FRAME = 0x21 -- Frame
local X_FLOATKEYS = 0x22 -- FloatKeys
local X_TIMEDFLOATKEYS = 0x23 -- TimedFloatKeys
local X_ANIMATIONKEY = 0x24 -- AnimationKey
local X_ANIMATIONOPTIONS = 0x25 -- AnimationOptions
local X_ANIMATION = 0x26 -- Animation
local X_ANIMATIONSET = 0x27 -- AnimationSet

local g_Defines = {
	{  X_ERROR,                 "error"                 },
	{  X_TEMPLATE,				"template"				},
	{  X_HEADER,				"Header"				},
	{  X_VECTOR,				"Vector"				},
	{  X_COORDS2D,				"Coords2d"				},
	{  X_QUATERNION,			"Quaternion"			},
	{  X_MATRIX4X4,				"Matrix4x4"				},
	{  X_COLORRGBA,				"ColorRGBA"				},
	{  X_COLORRGB,				"ColorRGB"				},
	{  X_INDEXEDCOLOR,			"IndexedColor"			},
	{  X_BOOLEAN,				"Boolean"				},
	{  X_BOOLEAN2D,				"Boolean2d"				},
	{  X_MATERIAL,				"Material"				},
	{  X_TEXTUREFILENAME,		"TextureFilename"		},
	{  X_MESHFACE,				"MeshFace"				},
	{  X_MESHFACEWRAPS,			"MeshFaceWraps"			},
	{  X_MESHTEXTURECOORDS,		"MeshTextureCoords"		},
	{  X_MESHNORMALS,			"MeshNormals"			},
	{  X_MESHVERTEXCOLORS,		"MeshVertexColors"		},
	{  X_MESHMATERIALLIST,		"MeshMaterialList"		},
	{  X_MESH,					"Mesh"					},
	{  X_FRAMETRANSFORMMATRIX,	"FrameTransformMatrix"	},
	{  X_FRAME,					"Frame"					},
	{  X_FLOATKEYS,				"FloatKeys"				},
	{  X_TIMEDFLOATKEYS,		"TimedFloatKeys"		},
	{  X_ANIMATIONKEY,			"AnimationKey"			},
	{  X_ANIMATIONOPTIONS,		"AnimationOptions"		},
	{  X_ANIMATION,				"Animation"				},
	{  X_ANIMATIONSET,			"AnimationSet"			}
}

-- use %n if the pattern uses ^ and $
local function StringLRTrim(s)
	if s == nil then return s end
	return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

local function RemoveComment(s)
	if s == nil then return s end
	return (string.gsub(s, "(//.*)|(#.*)", ""))
end

local function Getline(f)
	local s = f:read("*l")
	s = StringLRTrim(s)
	s = RemoveComment(s)
	return s
end

local function GetType(s)
	local word = (string.match(s, "%a+"))
	if word == nil then return false end
	for i = 1, #g_Defines do
		local item = g_Defines[i]
		if word == item[2] then
			return i
		end
	end
	return false
end

function readXfile(filename, hasDebugInfo)
	local f = assert(io.open(filename, "r"))
	local s = f:read("*l")
	assert(s == "xof 0303txt 0032",
		"Error - This file is not a compliant .x txt file")
	local lineno = 1
	local status = nil
	while true do
		local s = Getline(f)
		if s == nil then break end
		lineno = lineno + 1
		local type = GetType(s)
		if type ~= false then
			if hasDebugInfo then
				print(string.format(
					"[TYPE] %s:%d %s",
					filename, lineno, g_Defines[type][2]))
			end
			status = g_Defines[type][2]
		end
		if type == false and status == "Mesh" then
			print(s)
		end
	end
	f:close()
end

-- readXfile("miku.x")
-- readXfile("cubeA.x")
readXfile("cubeB.x")
