#include "pch.h"
#include "CppUnitTest.h"
#include "../InteractiveGraphicsSp2021/BaseObject.cpp"
#include "../InteractiveGraphicsSp2021/AbstractReader.cpp"
#include "../InteractiveGraphicsSp2021/TextFileReader.cpp"
#include "../InteractiveGraphicsSp2021/SceneReader.cpp"
#include <fstream>
using std::ofstream;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InteractiveGraphicsUnitTesting
{
   TEST_CLASS(ATextFileReader)
   {
   public:

      TEST_METHOD(ShouldHaveAnErrorIfTheFileWasNotFound)
      {
         TextFileReader sut("NoSuchFile.txt");
         sut.Open();
         Assert::IsTrue(sut.HasError());
      }

      TEST_METHOD(ShouldReportTheContentsOfAnExistingFile)
      {
         ofstream fout("TestFile.txt");
         fout << "Content Line 1" << std::endl
            << "Content Line 2" << std::endl;
         fout.close();

         TextFileReader sut("TestFile.txt");
         sut.Open();
         Assert::IsFalse(sut.HasError());
         sut.Read();
         string expected("Content Line 1\nContent Line 2\n");
         Assert::AreEqual(expected, sut.GetContents());

         std::remove("TestFile.txt");
      }
   };

   TEST_CLASS(ASceneReader)
   {
   public:

      TEST_METHOD(ShouldHaveAnErrorIfTheFileWasNotFound)
      {
         SceneReader sut("NoSuchFile.txt");
         sut.Open();
         Assert::IsTrue(sut.HasError());
      }

      TEST_METHOD(ShouldReportTheSingleCameraData)
      {
         ofstream fout("TestFile.txt");
         fout << "# camera name, pos x, pos y, pos z, fov, near plane, far plane" << std::endl;
         fout << "camera,1,2,3,60,0.1f,50" << std::endl;
         fout << "<endCameras>" << std::endl;
         fout.close();

         SceneReader sut("TestFile.txt");
         sut.Open();
         Assert::IsFalse(sut.HasError());
         sut.Read();
         vector<CameraData>& data = sut.GetCameraData();
         Assert::AreEqual(1, (int)data.size());
         string expected("camera");
         Assert::AreEqual(expected, data[0].name);
         Assert::AreEqual(1.0f, data[0].position.x);
         Assert::AreEqual(2.0f, data[0].position.y);
         Assert::AreEqual(3.0f, data[0].position.z);
         Assert::AreEqual(60.0f, data[0].fov);
         Assert::AreEqual(0.1f, data[0].nearPlane);
         Assert::AreEqual(50.0f, data[0].farPlane);

         std::remove("TestFile.txt");
      }

      TEST_METHOD(ShouldReportMultipleCameraData)
      {
         ofstream fout("TestFile.txt");
         fout << "# camera name, pos x, pos y, pos z, fov, near plane, far plane" << std::endl;
         fout << "camera,1,2,3,60,0.1f,50" << std::endl;
         fout << "camera2,1,2,3,60,0.1f,50" << std::endl;
         fout << "camera3,1,2,3,60,0.1f,50" << std::endl;
         fout << "<endCameras>" << std::endl;
         fout.close();

         SceneReader sut("TestFile.txt");
         sut.Open();
         Assert::IsFalse(sut.HasError());
         sut.Read();
         vector<CameraData>& data = sut.GetCameraData();
         Assert::AreEqual(3, (int)data.size());

         std::remove("TestFile.txt");
      }

      TEST_METHOD(ShouldReportTheSingleShaderData)
      {
         ofstream fout("TestFile.txt");
         fout << "# camera name, pos x, pos y, pos z, fov, near plane, far plane" << std::endl;
         fout << "camera,1,2,3,60,0.1f,50" << std::endl;
         fout << "<endCameras>" << std::endl;
         fout << "defaultShader,  default,default,none" << std::endl;
         fout << "<endShaders>" << std::endl;
         fout.close();

         SceneReader sut("TestFile.txt");
         sut.Open();
         Assert::IsFalse(sut.HasError());
         sut.Read();
         vector<ShaderData>& data = sut.GetShaderData();
         Assert::AreEqual(1, (int)data.size());
         string expected("defaultShader");
         Assert::AreEqual(expected, data[0].name);
         expected = "default";
         Assert::AreEqual(expected, data[0].vertexShaderFilePath);
         expected = "default";
         Assert::AreEqual(expected, data[0].fragmentShaderFilePath);
         expected = "none";
         Assert::AreEqual(expected, data[0].cameraName);

         std::remove("TestFile.txt");
      }

      TEST_METHOD(ShouldReportMultipleShaderData)
      {
         ofstream fout("TestFile.txt");
         fout << "# camera name, pos x, pos y, pos z, fov, near plane, far plane" << std::endl;
         fout << "camera,1,2,3,60,0.1f,50" << std::endl;
         fout << "<endCameras>" << std::endl;
         fout << "defaultShader,  default,default,none" << std::endl;
         fout << "simple3DShader,simple3Dvertex.glsl,default,camera" << std::endl;
         fout << "<endShaders>" << std::endl;
         fout.close();

         SceneReader sut("TestFile.txt");
         sut.Open();
         Assert::IsFalse(sut.HasError());
         sut.Read();
         vector<ShaderData>& data = sut.GetShaderData();
         Assert::AreEqual(2, (int)data.size());

         std::remove("TestFile.txt");
      }

   };
}
