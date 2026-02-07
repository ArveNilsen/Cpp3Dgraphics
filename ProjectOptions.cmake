# Ref: https://github.com/cpp-best-practices/cmake_template/blob/main/ProjectOptions.cmake

option(project_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)

macro(project_local_options)
    if(PROJECT_IS_TOP_LEVEL)
        include(cmake/StandardProjectSettings.cmake)
    endif()

    add_library(project_warnings INTERFACE)
    add_library(project_options INTERFACE)

    include(cmake/CompilerWarnings.cmake)
    project_set_project_warnings(
        project_warnings
        ${project_WARNINGS_AS_ERRORS}
        ""
        ""
        ""
        ""
    )

    include(cmake/StaticAnalyzers.cmake)
    if(project_ENABLE_CLANG_TIDY)
        project_enable_clang_tidy(project_options ${myproject_WARNINGS_AS_ERRORS})
    endif()

    if(myproject_ENABLE_CPPCHECK)
        project_enable_cppcheck(${project_WARNINGS_AS_ERRORS} "" # override cppcheck options
        )

  endif()
endmacro()
