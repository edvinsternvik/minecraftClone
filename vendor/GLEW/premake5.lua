project "GLEW"
	kind "StaticLib"
	language "C"
	
	targetdir "bin/"
	objdir "bin-int/"

	files {
		"include/**.h",
		"src/**.c"
	}

	includedirs {
		"include"
	}

	defines { "GLEW_STATIC" }