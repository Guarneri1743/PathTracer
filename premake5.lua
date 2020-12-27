local solution_dir = _ACTION
local lib_dir = "PathTracer/lib"
local res_dir = "PathTracer/res"
local src_dir = "PathTracer/src"
local third_party_dir = "PathTracer/third_party"
local sample_dir = "sample"

function setupIncludeDirs()
   includedirs {
      src_dir,
      src_dir .. "/math",
      src_dir .. "/primitives",
      src_dir .. "/utility",
      third_party_dir,
      third_party_dir .. "/threading",
      third_party_dir .. "/assimp",
      third_party_dir .. "/ply",
      third_party_dir .. "/stb_image",
      third_party_dir .. "/nlohmann"
   }
end

function setupSlotion()
   location(solution_dir)
   solution "PathTracer"
      configurations {
         "Debug", 
         "Release"
      }

      platforms { "Win64" }
      warnings "Extra"
      floatingpoint "Fast"
      symbols "On"
      cppdialect "C++17"
      rtti "On"
      characterset ("MBCS")

      configuration "Debug*"
         defines { "DEBUG", "_DEBUG" }
         targetdir ( solution_dir .. "lib/Debug" )

      configuration "Release*"
         defines { "NDEBUG" }
         optimize "On"
         targetdir ( solution_dir .. "lib/Release"  )

      filter { "platforms:Win*", "configurations:Debug*" }
         defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
         system "Windows"
         architecture "x86_64"
         staticruntime "Off"
         libdirs { lib_dir .. "/assimp/Debug" }
         local createResFolder = "mkdir \"$(OutDir)res\""
         local copyAssimp = "xcopy /y /d \"../" .. lib_dir .. "/assimp/Debug\" \"$(OutDir)\""
         local copyRes = "xcopy /y /d /s \"../" .. res_dir .. "\" \"$(OutDir)res\""
         postbuildcommands {
            createResFolder,
            copyAssimp,
            copyRes
         }

      filter { "platforms:Win*", "configurations:Release*" }
         defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
         system "Windows"
         architecture "x86_64"
         staticruntime "Off"
         libdirs { lib_dir .. "/assimp/Release" }
         local createResFolder = "mkdir \"$(OutDir)res\""
         local copyAssimp = "xcopy /y /d \"../" .. lib_dir .. "/assimp/Release\" \"$(OutDir)\""
         local copyRes = "xcopy /y /d /s \"../" .. res_dir .. "\" \"$(OutDir)res\""
         postbuildcommands {
            createResFolder,
            copyAssimp,
            copyRes
         }

      filter { "platforms:Win*", "configurations:Debug*" }
         links { "assimpd" }

      filter { "platforms:Win*", "configurations:Release*" }
         links { "assimp" }
end

function setupPathTracerDemo()
   project "PathTracer"
   kind "ConsoleApp"
   language "C++"

   files { 
      src_dir .. "/*.*", 
      src_dir .. "/math/*.*",
      src_dir .. "/primitives/*.*",
      src_dir .. "/utility/*.*",
      third_party_dir .. "/*.*",
      third_party_dir .. "/threading/*.*",
      third_party_dir .. "/assimp/*.*",
      third_party_dir .. "/stb_image/*.*",
      third_party_dir .. "/nlohmann/*.*",
      sample_dir .. "/cornell_box.cpp"
   }

   filter { "configurations:Debug*" }
      targetdir (solution_dir .. "/bin/Debug")

   filter { "configurations:Release*" }
      targetdir (solution_dir .. "/bin/release")
end

setupIncludeDirs()
setupSlotion()
setupPathTracerDemo()
