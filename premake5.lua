workspace "mcClone"
	architecture "x64"
	configurations { "Release" }

include("vendor/GLEW")
include("vendor/GLFW")

project "mcClone"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir "bin/"
	objdir "bin-int/"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"vendor/GLEW/include",
		"vendor/GLFW/include",
		"vendor/GLM/include",
		"vendor/stb"
	}

	links {
		"GLEW",
		"GLFW"
	}

	optimize "Speed"

	defines { "GLEW_STATIC" }

	filter "system:windows"
		links "opengl32.lib"

	filter "system:linux"
		linkoptions { "-lX11", "-ldl", "-lGL", "-lpthread" }