


releaseDir=release
debugDir=debug



all : $(releaseDir) $(debugDir)
	cd $(releaseDir) && cmake ..
	cd $(debugDir) && cmake .. -DCMAKE_CXX_FLAGS=-pg -DCMAKE_EXE_LINKER_FLAGS=-pg -DCMAKE_SHARED_LINKER_FLAGS=-pg


$(releaseDir) : 
	mkdir -p $(releaseDir)


$(debugDir) : 
	mkdir -p $(debugDir)


clean :
	rm -rf $(releaseDir) $(debugDir)





