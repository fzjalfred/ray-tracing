#!lua

includeDirList = {
    "../shared",
    "../shared/include",
    "../shared/gl3w",
    "../shared/imgui",
    "./my_share/include"
}

libDirectories = { 
    "../lib",
    "./my_share/lib"
}


if os.get() == "macosx" then
    linkLibs = {
        "cs488-framework",
        "imgui",
        "glfw3",
        "lua",
		"lodepng",
        "tbb"
    }
end

if os.get() == "linux" then
    linkLibs = {
        "cs488-framework",
        "lua",
        "lodepng",
        "stdc++",
        "dl",
        "pthread",
        "tbb"
    }
end

if os.get() == "macosx" then
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
end

buildOptions = {"-std=c++14 -O2 "}

solution "CS488-Projects"
    configurations { "Debug", "Release" }

    project "A4"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "*.cpp" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
