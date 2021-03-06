find_program(SPHINX_EXECUTABLE NAMES sphinx-build
    HINTS
    $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx documentation generator"
)

find_program(SPHINX_APIDOC_EXECUTABLE NAMES sphinx-apidoc
    HINTS
    $ENV{SPHINX_DIR}
    PATH_SUFFIXES bin
    DOC "Sphinx API generator"
)
 
include(FindPackageHandleStandardArgs)
 
find_package_handle_standard_args(Sphinx DEFAULT_MSG
    SPHINX_EXECUTABLE
)
 
find_package_handle_standard_args(Sphinx DEFAULT_MSG
    SPHINX_APIDOC_EXECUTABLE
)
 
mark_as_advanced(SPHINX_EXECUTABLE)
mark_as_advanced(SPHINX_APIDOC_EXECUTABLE)
