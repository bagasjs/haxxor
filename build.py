import os
import platform
from abc import ABC, abstractmethod
from pathlib import Path


class Helper:
	@staticmethod
	def rwildcard(path: str, cond: callable) -> list[str]:
		res = []
		for root, dirs, files in os.walk(path):
			res += [os.path.join(root, file) for file in files if cond(root, file)]
		return res
	
	@staticmethod
	def change_extension(path: str, ext: str) -> str:
		items = path.split(".")
		items[-1] = ext
		path = ".".join(items)
		return path
	
	@staticmethod
	def join_flag(items: list[str], flag: str):
		return f" {flag} ".join([""] + items)
	
	@staticmethod
	def path(*name):
		return str(Path(*name))
	
	@staticmethod
	def get_platform():
		return platform.system()

	
class CProject(ABC):
	KIND_EXECUTABLE = 0
	KIND_STATICLIB = 1
	KIND_SHAREDLIB = 2

	def __init__(self,
		NAME: str,
		CC: str,
		CFLAGS: str,
		KIND: str,
		SOURCES: list[str] = [],
		INCLUDES: list[str] = [],
		DEFINES: list[str] = [],
		LIBS: list[str] = [],
		LINKS: list[str] = []
		):
		self.NAME = NAME
		self.CC = CC
		self.CFLAGS = CFLAGS
		self.KIND = KIND
		self.SOURCES = SOURCES
		self.INCLUDES = INCLUDES
		self.DEFINES = DEFINES
		self.LIBS = LIBS
		self.LINKS = LINKS

		self._objdir = Helper.path("build/bin-int")
		self._targetdir = Helper.path("build/bin")
		self._dependencydir = Helper.path("build/dependencies")
		if not os.path.isdir(self._objdir) and not os.path.isfile(self._objdir):
			os.mkdir(self._objdir)
		if not os.path.isdir(self._targetdir) and not os.path.isfile(self._targetdir):
			os.mkdir(self._targetdir)
		if not os.path.isdir(self._dependencydir) and not os.path.isfile(self._dependencydir):
			os.mkdir(self._dependencydir)

	def get_objects(self):
		return [ os.path.join( self._objdir, Helper.change_extension(os.path.basename(file), "o")) for file in self.SOURCES ]

	@abstractmethod	
	def on_linux(self):
		pass

	@abstractmethod
	def on_windows(self):
		pass
	
	def build(self):
		platform = Helper.get_platform()
		executable_extension = "exe"
		if platform == "Windows":
			self.on_windows()
		elif platform == "Linux":
			executable_extension = "out"
			self.on_linux()
		else:
			raise Exception("Platform not supported")


		print("(*) Compilation Stage")
		objects = self.get_objects()
		rflags = Helper.join_flag(self.DEFINES, "-D") + Helper.join_flag(self.INCLUDES, "-I")
		for src, obj in dict(zip(self.SOURCES, objects)).items():
			print(f"Compiling {src}")
			os.system(f"{self.CC} {self.CFLAGS} -c {src} -o {obj} {rflags}")

		print("(*) Linking Stage")
		objects = Helper.join_flag(objects, '')
		prompt = "echo \"Failed to link\""
		if self.KIND == CProject.KIND_EXECUTABLE:
			target = Helper.path(self._targetdir, f"{self.NAME}.{executable_extension}")
			rflags = Helper.join_flag(self.LINKS, "-l") + Helper.join_flag(self.LIBS, "-L")
			prompt = f"{self.CC} {objects} -o {target} {rflags}"
		elif self.kind == CProject.KIND_STATICLIB:
			target = Helper.path(self._targetdir, f"lib{self.NAME}.a")
			prompt = f"ar rcs {target} {objects}"
		else:
			pass
		os.system(prompt)
		
		print("(*) Done")

if __name__ == "__main__":
	class HaxxorProject(CProject):
		def __init__(self):
			super().__init__(
				NAME = "haxxor",
				CC = "clang",
				CFLAGS = "-g -Wall -Werror",
				KIND = CProject.KIND_EXECUTABLE
			)

			self.SOURCES += [
				Helper.path(self._dependencydir, "src/context.c"),
				Helper.path(self._dependencydir, "src/init.c"),
				Helper.path(self._dependencydir, "src/input.c"),
				Helper.path(self._dependencydir, "src/monitor.c"),
				Helper.path(self._dependencydir, "src/vulkan.c"),
				Helper.path(self._dependencydir, "src/window.c")	
			] + Helper.rwildcard("./src", lambda path, file: file.endswith(".c"))

			self.INCLUDES += [
				"include",
				Helper.path(self._dependencydir, "include"),
				Helper.path(self._dependencydir, "include/glad"),
				Helper.path(self._dependencydir, "include/GLFW"),
				Helper.path(self._dependencydir, "include/KHR")
			]
			self.DEFINES += [
				"GLFW_USE_CONFIG_H"		
			]
		
		def on_linux(self):
			self.SOURCES += [
				Helper.path(self._dependencydir, "src/x11_init.c"),
				Helper.path(self._dependencydir,"src/x11_monitor.c"),
				Helper.path(self._dependencydir,"src/x11_window.c"),
				Helper.path(self._dependencydir,"src/xkb_unicode.c"),
				Helper.path(self._dependencydir, "src/posix_time.c"),
				Helper.path(self._dependencydir, "src/posix_thread.c"),
				Helper.path(self._dependencydir, "src/glx_context.c"),
				Helper.path(self._dependencydir, "src/egl_context.c"),
				Helper.path(self._dependencydir, "src/osmesa_context.c"),
				Helper.path(self._dependencydir, "src/linux_joystick.c"),
				Helper.path(self._dependencydir, "src/glad.c"),
			]
			self.DEFINES += ["_GLFW_X11"]
			self.LINKS += [
				"X11",
				"m" # math
			]

		def on_windows(self):
			self.SOURCES += [
				Helper.path(self._dependencydir, "src/win32_init.c"),
				Helper.path(self._dependencydir, "src/win32_joystick.c"),
				Helper.path(self._dependencydir, "src/win32_monitor.c"),
				Helper.path(self._dependencydir, "src/win32_time.c"),
				Helper.path(self._dependencydir, "src/win32_thread.c"),
				Helper.path(self._dependencydir, "src/win32_window.c"),
				Helper.path(self._dependencydir, "src/wgl_context.c"),
				Helper.path(self._dependencydir, "src/egl_context.c"),
				Helper.path(self._dependencydir, "src/osmesa_context.c")
			]
			self.DEFINES += [
				"_GLFW_WIN32",
				"_CRT_SECURE_NO_WARNINGS"
			]

	project = HaxxorProject()
	project.build()