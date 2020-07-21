compile_arguments = -std=c++17 -luser32

builds/autotempdel.exe: builds/objects/auto_temp_del.o
	mkdir -p builds/objects
	clang++ builds/objects/auto_temp_del.o -o builds/autotempdel.exe $(compile_arguments)

builds/objects/auto_temp_del.o: source/auto_temp_del.cxx
	mkdir -p builds/objects
	clang++ source/auto_temp_del.cxx -c -o builds/objects/auto_temp_del.o $(compile_arguments)
