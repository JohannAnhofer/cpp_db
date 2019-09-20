add_executable(vsunittests
	stdafx.h
	stdafx.cpp
	targetver.h
	unittest_cpp_db.cpp
)

target_link_libraries(vsunittests PRIVATE
    tools
    postgres
    firebird
    sqlite
    cpp_db
    )
