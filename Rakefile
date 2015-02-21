#Rake.application.options.trace_rules = true
require "rake/clean"
require 'rake/loaders/makefile'
require 'tempfile'

TARGET = "multicopter"

CORE_LIBRARIES = ["SPI"]

# Arduino IDE installation location
ARDUINO_HOME = "/Applications/Arduino.app/Contents/Resources/Java"

# Configurable options
OPTIONS = [
  "-DF_CPU=96000000",
  "-DUSB_SERIAL",
  "-DLAYOUT_US_ENGLISH",
  "-D__MK20DX256__",
  "-DARDUINO=106",
  "-DTEENSYDUINO=120"
]

#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

# Arduino/Teensy files, Core libs
ARDUINO_HW_HOME = "#{ARDUINO_HOME}/hardware"
CORE_LIB_HOME = "#{ARDUINO_HOME}/libraries"
TEENSY_HOME = "#{ARDUINO_HW_HOME}/teensy/cores/teensy3"

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLS_PATH = "#{ARDUINO_HW_HOME}/tools"

# path location for the arm-none-eabi compiler
COMPILER_PATH = "#{ARDUINO_HW_HOME}/tools/arm-none-eabi/bin"

# where to store build output files
OBJ_DIR = ".build"

#************************************************************************
# Library includes and sources
#************************************************************************

SOURCE_FILES = FileList.new do |fl|
  CORE_LIBRARIES.each do |lib|
    fl.include "#{CORE_LIB_HOME}/#{lib}/*.c", "#{CORE_LIB_HOME}/#{lib}/*.cpp"
  end
  fl.include "./**/*.c", "./**/*.cpp"
  fl.include "#{TEENSY_HOME}/**/*.c", "#{TEENSY_HOME}/**/*.cpp"
  fl.exclude "#{TEENSY_HOME}/main.cpp"
end
OBJECT_FILES = SOURCE_FILES.pathmap("#{OBJ_DIR}/%n.o")
INCLUDE_PATHS = SOURCE_FILES.pathmap("%d").uniq
HEX_FILE = "#{OBJ_DIR}/#{TARGET}.hex"
ELF_FILE = "#{OBJ_DIR}/#{TARGET}.elf"

# puts SOURCE_FILES
# puts
# puts
# puts INCLUDE_PATHS
# puts
# puts
# puts OBJECT_FILES
# puts
# puts

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = [
  "-Wall",
  "-g",
  "-Os",
  "-mcpu=cortex-m4",
  "-mthumb",
  "-nostdlib",
  #"-MMD",
  "-fdata-sections",
  "-ffunction-sections",
  OPTIONS,
  INCLUDE_PATHS.map { |path| "-I#{path}" }
].flatten.join(" ")

# compiler options for C++ only
CXXFLAGS = [
  "-std=gnu++0x",
  "-felide-constructors",
  "-fno-exceptions",
  "-fno-rtti"
].join(" ")

# compiler options for C only
CFLAGS = [].join(" ")

# linker options
LDFLAGS = [
  "-Os",
  "-Wl,--gc-sections",
  "-mcpu=cortex-m4",
  "-mthumb",
  "-T#{TEENSY_HOME}/mk20dx256.ld"
].join(" ")

# additional libraries to link
LIBS = [
  "-lm",
  "-larm_cortexM4l_math"
].join(" ")

# names for the compiler programs
CC = "#{COMPILER_PATH}/arm-none-eabi-gcc"
CXX = "#{COMPILER_PATH}/arm-none-eabi-g++"
OBJCOPY = "#{COMPILER_PATH}/arm-none-eabi-objcopy"
SIZE = "#{COMPILER_PATH}/arm-none-eabi-size"

directory OBJ_DIR
CLEAN.include(OBJ_DIR)

import "#{OBJ_DIR}/.depends.mf"
#Rake::MakefileLoader.new.load(".depends.mf") if File.file?(".depends.mf")

task :default => HEX_FILE

task :burn => HEX_FILE do
  sh "~/hardware/teensy-loader-cli/teensy_loader_cli -w -v -mmcu=mk20dx256 #{HEX_FILE}"
end

file HEX_FILE => [OBJ_DIR, ELF_FILE] do
  sh "#{SIZE} #{ELF_FILE}"
  sh "#{OBJCOPY} -O ihex -R .eeprom #{ELF_FILE} #{HEX_FILE}"
end

file ELF_FILE => OBJECT_FILES do
  sh "#{CC} #{LDFLAGS} -o #{ELF_FILE} #{OBJECT_FILES} #{LIBS}"
end

rule ".o" => ->(name){ lookup_source_file(name) } do |t|
  puts "COMPL #{t.source} -> #{t.name}"

  if File.extname(t.source) == ".c"
    sh "#{CC} -c #{CPPFLAGS} #{CFLAGS} -o \"#{t.name}\" \"#{t.source}\""
  else
    sh "#{CXX} -c #{CPPFLAGS} #{CXXFLAGS} -o \"#{t.name}\" \"#{t.source}\""
  end
end

file "#{OBJ_DIR}/.depends.mf" => SOURCE_FILES do |t|
  mkdir_p OBJ_DIR
  sh "makedepend -f- -- #{CPPFLAGS} -- #{t.prerequisites.join(" ")} > #{t.name}"

  path = "#{OBJ_DIR}/.depends.mf"
  temp_file = Tempfile.new(".depends.mf.temp")
  File.open(path, 'r') do |file|
    file.each_line do |line|
      next if line =~ /#/ || line =~ /^$/
      file = line[/^[^:]+/]
      temp_file.puts line.sub(file, File.join(OBJ_DIR, File.basename(file)))
    end
  end
  temp_file.close
  FileUtils.mv(temp_file.path, path)
end

def lookup_source_file(file)
  SOURCE_FILES.detect { |f| File.basename(f).ext("") == File.basename(file).ext("") }
end
