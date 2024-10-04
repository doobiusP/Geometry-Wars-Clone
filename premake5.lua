workspace "OpenGL_Games"
   configurations { "Debug", "Release" }
   architecture "x86_64"

newoption {
   trigger = "mt",
   description = "Use static runtime (only valid for msc toolset). Use if using Visual Studio with static runtime linking as /MT or /MTd"
}

project "Geometry_Game"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"

   targetname ("geomg")
   files { "headers/**.hpp", "src/**.cpp" }
   includedirs { "headers/", "includes/" }
   libdirs { "libs/" }
   defines { "GLEW_STATIC" }
   links { "glfw3", "gdi32", "glew32s", "opengl32" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      postbuildcommands {
         "cmd /c bin\\Debug\\geomg.exe $(ARGS)",  
         "{COPY} shaders bin\\Debug\\shaders"    
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"
      postbuildcommands {
         "cmd /c bin\\Release\\geomg.exe $(ARGS)", 
         "{COPY} shaders bin\\Release\\shaders"  
      }
