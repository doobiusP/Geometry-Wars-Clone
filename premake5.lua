workspace "OpenGL_Games"
   configurations { "Debug", "Release" }
   architecture "x86_64"

project "Geometry_Game"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   targetname ("geomg")
   files { "headers/**.hpp", "src/**.cpp" }
   includedirs { "headers/", "includes/" }
   libdirs { "libs/" }
   defines { "GLEW_STATIC" }
   links {
      "glfw3",
      "gdi32",
      "glew32s",
      "opengl32",
   }
   buildoptions { "-w" ,
                  "--std=c++20",
                  "-m64"}

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      postbuildcommands {
         "cmd /c bin\\Debug\\geomg.exe $(ARGS)"  -- Command to run the executable
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"
      postbuildcommands {
         "cmd /c bin\\Release\\geomg.exe $(ARGS)"  -- Command to run the executable
      }
