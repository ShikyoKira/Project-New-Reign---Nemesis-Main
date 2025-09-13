cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project("Nemesis_Engine" VERSION 1.0.0)

include_directories(include)

################################################################################
# Required packages
################################################################################
if(WIN32)
    include(Python.cmake)
else()
    find_package(Python EXACT 3.12.11 COMPONENTS Interpreter Development.Embed REQUIRED)
    add_library(Nemesis::Python ALIAS Python::Python) 
endif()

################################################################################
# Header Files
################################################################################
set(AlternateAnimation_HeaderFiles
    "include/AlternateAnimation/AlterAnim.h"
    "include/AlternateAnimation/AlterAnimGroup.h"
    "include/AlternateAnimation/AlterAnimPrefix.h"
    "include/AlternateAnimation/AlterAnimRepository.h"
    "include/AlternateAnimation/AlterAnimSet.h"
    )
source_group("Header Files\\AlternateAnimation" FILES ${AlternateAnimation_HeaderFiles})

set(AnimationData_HeaderFiles
    "include/Core/AnimationData/AnimationDataClipData.h"
    "include/Core/AnimationData/AnimationDataMotionData.h"
    "include/Core/AnimationData/AnimationDataProject.h"
    "include/Core/AnimationData/AnimationDataSingleFile.h"
    )
source_group("Header Files\\Core\\AnimationData" FILES ${AnimationData_HeaderFiles})

set(AnimationSetData_HeaderFiles
    "include/Core/AnimationSetData/AnimationSetDataProject.h"
    "include/Core/AnimationSetData/AnimationSetDataSingleFile.h"
    "include/Core/AnimationSetData/AnimationSetDataState.h"
    )
source_group("Header Files\\Core\\AnimationSetData" FILES ${AnimationSetData_HeaderFiles})

set(Hkx_HeaderFiles
    "include/Core/Hkx/HkxBehavior.h"
    "include/Core/Hkx/HkxCharacter.h"
    "include/Core/Hkx/HkxFile.h"
    "include/Core/Hkx/HkxNode.h"
    "include/Core/Hkx/HkxPlatform.h"
    )
source_group("Header Files\\Core\\Hkx" FILES ${Hkx_HeaderFiles})

set(LineModifier_HeaderFiles
    "include/Core/LineModifier/AnimationBaseFilenameModifier.h"
    "include/Core/LineModifier/AnimationEventModifier.h"
    "include/Core/LineModifier/AnimationFilenameModifier.h"
    "include/Core/LineModifier/AnimationFilePathModifier.h"
    "include/Core/LineModifier/AnimationOrderModifier.h"
    "include/Core/LineModifier/AttributeIdModifier.h"
    "include/Core/LineModifier/BaseIdModifier.h"
    "include/Core/LineModifier/CounterModifier.h"
    "include/Core/LineModifier/Crc32Modifier.h"
    "include/Core/LineModifier/CurrentCountModifier.h"
    "include/Core/LineModifier/EventIdModifier.h"
    "include/Core/LineModifier/GroupSelectionModifier.h"
    "include/Core/LineModifier/LengthModifier.h"
    "include/Core/LineModifier/LineModifier.h"
    "include/Core/LineModifier/LineModifierCollection.h"
    "include/Core/LineModifier/LineModifierFactory.h"
    "include/Core/LineModifier/ListNameModifier.h"
    "include/Core/LineModifier/MapModifier.h"
    "include/Core/LineModifier/MathModifier.h"
    "include/Core/LineModifier/MaxModifier.h"
    "include/Core/LineModifier/MinModifier.h"
    "include/Core/LineModifier/MotionDataModifier.h"
    "include/Core/LineModifier/MultipleChoiceModifier.h"
    "include/Core/LineModifier/NumelementModifier.h"
    "include/Core/LineModifier/OptionVariableModifier.h"
    "include/Core/LineModifier/PropertyIdModifier.h"
    "include/Core/LineModifier/RequestIdModifier.h"
    "include/Core/LineModifier/RequestIndexModifier.h"
    "include/Core/LineModifier/RotationDataModifier.h"
    "include/Core/LineModifier/RunningNumberModifier.h"
    "include/Core/LineModifier/StandardLineModifier.h"
    "include/Core/LineModifier/StateIdModifier.h"
    "include/Core/LineModifier/SubstringModifier.h"
    "include/Core/LineModifier/SubTemplateModifier.h"
    "include/Core/LineModifier/VariableIdModifier.h"
    )
source_group("Header Files\\Core\\LineModifier" FILES ${LineModifier_HeaderFiles})

set(Statement_HeaderFiles
    "include/Core/Statement/AnimationBaseFilenameStatement.h" 
    "include/Core/Statement/AnimationEventStatement.h"
    "include/Core/Statement/AnimationFilenameStatement.h" 
    "include/Core/Statement/AnimationFilePathStatement.h"
    "include/Core/Statement/AnimationOrderStatement.h"
    "include/Core/Statement/AttributeIdStatement.h"
    "include/Core/Statement/BaseIdStatement.h"
    "include/Core/Statement/CompositeStatement.h"
    "include/Core/Statement/ConditionalStatement.h"
    "include/Core/Statement/Crc32Statement.h"
    "include/Core/Statement/ElseIfStatement.h"
    "include/Core/Statement/EventIdStatement.h"
    "include/Core/Statement/ForEachStatement.h"
    "include/Core/Statement/IfStatement.h"
    "include/Core/Statement/LengthStatement.h"
    "include/Core/Statement/ListNameStatement.h"
    "include/Core/Statement/MapStatement.h"
    "include/Core/Statement/MathStatement.h"
    "include/Core/Statement/MaxStatement.h"
    "include/Core/Statement/MinStatement.h"
    "include/Core/Statement/ModCodeStatement.h"
    "include/Core/Statement/MotionDataStatement.h"
    "include/Core/Statement/MultipleChoiceStatement.h"
    "include/Core/Statement/OptionStatement.h"
    "include/Core/Statement/OptionVariableStatement.h"
    "include/Core/Statement/PropertyIdStatement.h"
    "include/Core/Statement/RequestIdStatement.h"
    "include/Core/Statement/RequestIndexStatement.h"
    "include/Core/Statement/RotationDataStatement.h"
    "include/Core/Statement/RunningNumberStatement.h"
    "include/Core/Statement/StateIdStatement.h"
    "include/Core/Statement/Statement.h"
    "include/Core/Statement/SubstringStatement.h"
    "include/Core/Statement/SubTemplateStatement.h"
    "include/Core/Statement/VariableIdStatement.h"
    )
source_group("Header Files\\Core\\Statements" FILES ${Statement_HeaderFiles})

set(Template_HeaderFiles
    "include/Core/Template/TemplateAnimDataClipData.h"
    "include/Core/Template/TemplateAnimDataMotionData.h"
    "include/Core/Template/TemplateClass.h"
    "include/Core/Template/TemplateHkx.h"
    "include/Core/Template/TemplateObject.h"
    "include/Core/Template/TemplateOption.h"
    "include/Core/Template/TemplateOptionModel.h"
    "include/Core/Template/TemplateRepository.h"
    )
source_group("Header Files\\Core\\Template" FILES ${Template_HeaderFiles})

set(Core_HeaderFiles
    "include/Core/AnimationRequest.h"
    "include/Core/AnimationRequestCollection.h"
    "include/Core/AnimationRequestRepository.h"
    "include/Core/BreakObject.h"
    "include/Core/CacheEntry.h"
    "include/Core/CacheManager.h" 
    "include/Core/CollectionObject.h"
    "include/Core/CompilationManager.h"
    "include/Core/CompileState.h"
    "include/Core/CoreObject.h"
    "include/Core/EmptyObject.h"
    "include/Core/FNISaaPexFile.h"
    "include/Core/ForEachObject.h"
    "include/Core/HashableObject.h"
    "include/Core/IfObject.h"
    "include/Core/LineStream.h"
    "include/Core/ModClass.h"
    "include/Core/ModInfo.h"
    "include/Core/ModLine.h"
    "include/Core/ModObject.h"
    "include/Core/ModPatch.h"
    "include/Core/ModRepository.h"
    "include/Core/NLine.h"
    "include/Core/NObject.h"
    "include/Core/NObjectRepository.h"
    "include/Core/ParsingForEachScope.h"
    "include/Core/SemanticManager.h"
    "include/Core/SubTemplateObject.h"
    "include/Core/SubTemplateRequest.h"
    "include/Core/Template.h"
    )
source_group("Header Files\\Core" FILES ${Core_HeaderFiles})

set(ExAnimation_HeaderFiles
    "include/ExAnimation/ExAnimationPack.h"
    "include/ExAnimation/ExAnimationProject.h"
    "include/ExAnimation/ExAnimationRepository.h"
    "include/ExAnimation/ExAnimationRequest.h"
    "include/ExAnimation/LinkedExAnimList.h"
    )
source_group("Header Files\\ExAnimation" FILES ${ExAnimation_HeaderFiles})

set(Utilities_HeaderFiles
    "include/Utilities/Algorithm.h"
    "include/Utilities/Compute.h"
    "include/Utilities/ConditionSyntax.h"
    "include/Utilities/Crc32.h"
    "include/Utilities/Exception.h"
    "include/Utilities/File.h"
    "include/Utilities/LimitedConcurrency.h"
    "include/Utilities/Line.h"
    "include/Utilities/Lockless.h"
    "include/Utilities/MD5.h"
    "include/Utilities/NonCopyable.h"
    "include/Utilities/OnScopeEnds.h"
    "include/Utilities/ProgressMeter.h"
    "include/Utilities/SharableWrapper.h"
    "include/Utilities/Sha256.h"
    "include/Utilities/StringExtension.h"
    "include/Utilities/StringMatchTrie.h"
    "include/Utilities/ThreadPool.h"
    "include/Utilities/Trie.h"
    "include/Utilities/Types.h"
    )
source_group("Header Files\\Utilities" FILES ${Utilities_HeaderFiles})

set(HeaderFiles
    "include/Logger.h"
    "include/NemesisInfo.h"
    )
source_group("Header Files" FILES ${HeaderFiles})

set(AllHeaderFiles
    ${AlternateAnimation_HeaderFiles}
    ${AnimationData_HeaderFiles}
    ${AnimationSetData_HeaderFiles}
    ${Hkx_HeaderFiles}
    ${LineModifier_HeaderFiles}
    ${Statement_HeaderFiles}
    ${Template_HeaderFiles}
    ${Core_HeaderFiles}
    ${ExAnimation_HeaderFiles}
    ${Utilities_HeaderFiles}
    ${HeaderFiles}
    )


################################################################################
# Source Files
################################################################################
set(AlternateAnimation_SourceFiles
    "src/AlternateAnimation/AlterAnim.cpp"
    "src/AlternateAnimation/AlterAnimGroup.cpp"
    "src/AlternateAnimation/AlterAnimPrefix.cpp"
    "src/AlternateAnimation/AlterAnimRepository.cpp"
    "src/AlternateAnimation/AlterAnimSet.cpp"
    )
source_group("Source Files\\AlternateAnimation" FILES ${AlternateAnimation_SourceFiles})

set(AnimationData_SourceFiles
    "src/Core/AnimationData/AnimationDataProject.cpp"
    "src/Core/AnimationData/AnimationDataClipData.cpp"
    "src/Core/AnimationData/AnimationDataMotionData.cpp"
    "src/Core/AnimationData/AnimationDataSingleFile.cpp"
    )
source_group("Source Files\\Core\\AnimationData" FILES ${AnimationData_SourceFiles})

set(AnimationSetData_SourceFiles
    "src/Core/AnimationSetData/AnimationSetDataState.cpp"
    "src/Core/AnimationSetData/AnimationSetDataProject.cpp"
    "src/Core/AnimationSetData/AnimationSetDataSingleFile.cpp"
    )
source_group("Source Files\\Core\\AnimationSetData" FILES ${AnimationSetData_SourceFiles})

set(Hkx_SourceFiles
    "src/Core/Hkx/HkxBehavior.cpp"
    "src/Core/Hkx/HkxCharacter.cpp"
    "src/Core/Hkx/HkxFile.cpp"
    "src/Core/Hkx/HkxNode.cpp"
    )
source_group("Source Files\\Core\\Hkx" FILES ${Hkx_SourceFiles})

set(LineModifier_SourceFiles
    "src/Core/LineModifier/AnimationBaseFilenameModifier.cpp"
    "src/Core/LineModifier/AnimationEventModifier.cpp"
    "src/Core/LineModifier/AnimationFilenameModifier.cpp"
    "src/Core/LineModifier/AnimationFilePathModifier.cpp"
    "src/Core/LineModifier/AnimationOrderModifier.cpp"
    "src/Core/LineModifier/AttributeIdModifier.cpp"
    "src/Core/LineModifier/CounterModifier.cpp"
    "src/Core/LineModifier/Crc32Modifier.cpp"
    "src/Core/LineModifier/CurrentCountModifier.cpp"
    "src/Core/LineModifier/EventIdModifier.cpp"
    "src/Core/LineModifier/GroupSelectionModifier.cpp"
    "src/Core/LineModifier/LengthModifier.cpp"
    "src/Core/LineModifier/LineModifier.cpp"
    "src/Core/LineModifier/LineModifierCollection.cpp"
    "src/Core/LineModifier/LineModifierFactory.cpp"
    "src/Core/LineModifier/ListNameModifier.cpp"
    "src/Core/LineModifier/MapModifier.cpp"
    "src/Core/LineModifier/MathModifier.cpp"
    "src/Core/LineModifier/MaxModifier.cpp"
    "src/Core/LineModifier/MinModifier.cpp"
    "src/Core/LineModifier/MotionDataModifier.cpp"
    "src/Core/LineModifier/MultipleChoiceModifier.cpp"
    "src/Core/LineModifier/NumelementModifier.cpp"
    "src/Core/LineModifier/OptionVariableModifier.cpp"
    "src/Core/LineModifier/PropertyIdModifier.cpp"
    "src/Core/LineModifier/RequestIdModifier.cpp"
    "src/Core/LineModifier/RequestIndexModifier.cpp"
    "src/Core/LineModifier/RotationDataModifier.cpp"
    "src/Core/LineModifier/RunningNumberModifier.cpp"
    "src/Core/LineModifier/StateIdModifier.cpp"
    "src/Core/LineModifier/SubstringModifier.cpp"
    "src/Core/LineModifier/SubTemplateModifier.cpp"
    "src/Core/LineModifier/VariableIdModifier.cpp"
    )
source_group("Source Files\\Core\\LineModifier" FILES ${LineModifier_SourceFiles})

set(Statement_SourceFiles
    "src/Core/Statement/AnimationBaseFilenameStatement.cpp"
    "src/Core/Statement/AnimationEventStatement.cpp"
    "src/Core/Statement/AnimationFilenameStatement.cpp"
    "src/Core/Statement/AnimationFilePathStatement.cpp"
    "src/Core/Statement/AnimationOrderStatement.cpp"
    "src/Core/Statement/AttributeIdStatement.cpp"
    "src/Core/Statement/BaseIdStatement.cpp"
    "src/Core/Statement/CompositeStatement.cpp"
    "src/Core/Statement/ConditionalStatement.cpp"
    "src/Core/Statement/Crc32Statement.cpp"
    "src/Core/Statement/ElseIfStatement.cpp"
    "src/Core/Statement/EventIdStatement.cpp"
    "src/Core/Statement/ForEachStatement.cpp"
    "src/Core/Statement/IfStatement.cpp"
    "src/Core/Statement/LengthStatement.cpp"
    "src/Core/Statement/ListNameStatement.cpp"
    "src/Core/Statement/MapStatement.cpp"
    "src/Core/Statement/MathStatement.cpp"
    "src/Core/Statement/MaxStatement.cpp"
    "src/Core/Statement/MinStatement.cpp"
    "src/Core/Statement/ModCodeStatement.cpp"
    "src/Core/Statement/MotionDataStatement.cpp"
    "src/Core/Statement/MultipleChoiceStatement.cpp"
    "src/Core/Statement/OptionStatement.cpp"
    "src/Core/Statement/OptionVariableStatement.cpp"
    "src/Core/Statement/PropertyIdStatement.cpp"
    "src/Core/Statement/RequestIdStatement.cpp"
    "src/Core/Statement/RequestIndexStatement.cpp"
    "src/Core/Statement/RotationDataStatement.cpp"
    "src/Core/Statement/RunningNumberStatement.cpp"
    "src/Core/Statement/StateIdStatement.cpp"
    "src/Core/Statement/Statement.cpp"
    "src/Core/Statement/SubstringStatement.cpp"
    "src/Core/Statement/SubTemplateStatement.cpp"
    "src/Core/Statement/VariableIdStatement.cpp"
    )
source_group("Source Files\\Core\\Statements" FILES ${Statement_SourceFiles})

set(Template_SourceFiles
    "src/Core/Template/TemplateAnimDataClipData.cpp"
    "src/Core/Template/TemplateAnimDataMotionData.cpp"
    "src/Core/Template/TemplateClass.cpp"
    "src/Core/Template/TemplateHkx.cpp"
    "src/Core/Template/TemplateObject.cpp"
    "src/Core/Template/TemplateOption.cpp"
    "src/Core/Template/TemplateOptionModel.cpp"
    "src/Core/Template/TemplateRepository.cpp"
    )
source_group("Source Files\\Core\\Template" FILES ${Template_SourceFiles})

set(Core_SourceFiles
    "src/Core/AnimationRequest.cpp"
    "src/Core/AnimationRequestRepository.cpp"
    "src/Core/BreakObject.cpp"
    "src/Core/CacheManager.cpp"
    "src/Core/CollectionObject.cpp"
    "src/Core/CompilationManager.cpp"
    "src/Core/CompileState.cpp"
    "src/Core/EmptyObject.cpp"
    "src/Core/FNISaaPexFile.cpp"
    "src/Core/ForEachObject.cpp"
    "src/Core/IfObject.cpp"
    "src/Core/LineStream.cpp"
    "src/Core/ModClass.cpp"
    "src/Core/ModInfo.cpp"
    "src/Core/ModLine.cpp"
    "src/Core/ModObject.cpp"
    "src/Core/ModRepository.cpp"
    "src/Core/NLine.cpp"
    "src/Core/NObject.cpp"
    "src/Core/NObjectRepository.cpp"
    "src/Core/ParsingForEachScope.cpp"
    "src/Core/SemanticManager.cpp"
    "src/Core/SubTemplateObject.cpp"
    "src/Core/SubTemplateRequest.cpp"
    )
source_group("Source Files\\Core" FILES ${Core_SourceFiles})

set(ExAnimation_SourceFiles
    "src/ExAnimation/ExAnimationPack.cpp"
    "src/ExAnimation/ExAnimationProject.cpp"
    "src/ExAnimation/ExAnimationRepository.cpp"
    "src/ExAnimation/ExAnimationRequest.cpp"
    "src/ExAnimation/LinkedExAnimList.cpp"
    )
source_group("Source Files\\ExAnimation" FILES ${ExAnimation_SourceFiles})

set(Utilities_SourceFiles
    "src/Utilities/Algorithm.cpp"
    "src/Utilities/Compute.cpp"
    "src/Utilities/ConditionSyntax.cpp"
    "src/Utilities/Crc32.cpp"
    "src/Utilities/File.cpp"
    "src/Utilities/LimitedConcurrency.cpp"
    "src/Utilities/Line.cpp"
    "src/Utilities/MD5.cpp"
    "src/Utilities/ProgressMeter.cpp"
    "src/Utilities/StringExtension.cpp"
    "src/Utilities/ThreadPool.cpp"
    )
source_group("Source Files\\Utilities" FILES ${Utilities_SourceFiles})

set(SourceFiles
    "src/Logger.cpp"
    "src/main.cpp"
    "src/NemesisInfo.cpp"
    )
source_group("Source Files" FILES ${SourceFiles})

set(AllSourceFiles
    ${AlternateAnimation_SourceFiles}
    ${AnimationData_SourceFiles}
    ${AnimationSetData_SourceFiles}
    ${Hkx_SourceFiles}
    ${LineModifier_SourceFiles}
    ${Statement_SourceFiles}
    ${Template_SourceFiles}
    ${Core_SourceFiles}
    ${ExAnimation_SourceFiles}
    ${Utilities_SourceFiles}
    ${SourceFiles}
    )


################################################################################
# Fetch
################################################################################
include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    exprtk
    GIT_REPOSITORY https://github.com/ArashPartow/exprtk.git
    GIT_TAG 0.0.3
)
FetchContent_MakeAvailable(exprtk)
include_directories(${exprtk_SOURCE_DIR})

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${AllHeaderFiles} ${AllSourceFiles})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        nlohmann_json::nlohmann_json
        Nemesis::Havok
        Nemesis::Python
)

if(MSVC)
    #Required
    target_compile_options(${PROJECT_NAME} PRIVATE "/bigobj")
    target_link_options(${PROJECT_NAME} PRIVATE "/WHOLEARCHIVE:$<TARGET_FILE:Nemesis::Havok>")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${Python_DLL_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
elseif(AppleClang)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-force_load,$<TARGET_FILE:Nemesis::Havok>")
else()
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--whole-archive,$<TARGET_FILE:Nemesis::Havok>,--no-whole-archive")
endif()
