from conans import ConanFile, CMake

class Shapes(ConanFile):
   settings = "os", "compiler", "build_type", "arch", "arch_build"
   
   generators = "cmake"
   
   build_requires = (
    "pybind11/[>=2.6.1]"
   )


   def imports(self):
      self.copy("*.dll", dst="bin", src="bin") # From bin to bin
      self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
      self.copy("*.so*", dst="bin", src="lib") # From lib to bin
   
   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()