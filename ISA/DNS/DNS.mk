##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=DNS
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=/home/pdancak/school/MrPain_T/ISA/DNS/DNS
ProjectPath            :=/home/pdancak/school/MrPain_T/ISA/DNS
IntermediateDirectory  :=DNS/build-$(ConfigurationName)/__
OutDir                 :=DNS/build-$(ConfigurationName)/__
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=pdancak
Date                   :=20/10/20
CodeLitePath           :=/home/pdancak/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=DNS/build-$(ConfigurationName)/bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
LinkOptions            :=  -pthread
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=DNS/build-$(ConfigurationName)/__/main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): DNS/build-$(ConfigurationName)/__/.d $(Objects) 
	@mkdir -p "DNS/build-$(ConfigurationName)/__"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@mkdir -p "DNS/build-$(ConfigurationName)/__"
	@mkdir -p ""DNS/build-$(ConfigurationName)/bin""

DNS/build-$(ConfigurationName)/__/.d:
	@mkdir -p "DNS/build-$(ConfigurationName)/__"

PreBuild:


##
## Objects
##
DNS/build-$(ConfigurationName)/__/main.cpp$(ObjectSuffix): main.cpp DNS/build-$(ConfigurationName)/__/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/pdancak/school/MrPain_T/ISA/DNS/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
DNS/build-$(ConfigurationName)/__/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MTDNS/build-$(ConfigurationName)/__/main.cpp$(ObjectSuffix) -MFDNS/build-$(ConfigurationName)/__/main.cpp$(DependSuffix) -MM main.cpp

DNS/build-$(ConfigurationName)/__/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) DNS/build-$(ConfigurationName)/__/main.cpp$(PreprocessSuffix) main.cpp


-include DNS/build-$(ConfigurationName)/__//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


