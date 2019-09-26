enable_testing()

add_library(vsunittests
	stdafx.h
	stdafx.cpp
	targetver.h
	unittest_cpp_db.cpp
)
message("Includepath: $ENV{VSINSTALLDIR}\\VC\\Auxiliary\\VS\\UnitTest\\include")
target_include_directories(vsunittests 
	PRIVATE 
		"$ENV{VSINSTALLDIR}\\VC\\Auxiliary\\VS\\UnitTest\\include"
)

target_link_directories(vsunittests 
	PRIVATE
		"$ENV{VSINSTALLDIR}\\VC\\Auxiliary\\VS\\UnitTest\\lib\\"	
)

target_compile_definitions(vsunittests PRIVATE "_UNICODE" "UNICODE")
target_link_libraries(vsunittests 
	PRIVATE
		tools
		postgres
		firebird
		sqlite
		cpp_db
)
