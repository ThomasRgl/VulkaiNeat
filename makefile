


releaseDir=release
debugDir=debug



all : $(releaseDir) $(debugDir)
	cd $(releaseDir) && cmake ..
	cd $(debugDir) && cmake .. -DCMAKE_BUILD_TYPE=Debug


$(releaseDir) : 
	mkdir -p $(releaseDir)


$(debugDir) : 
	mkdir -p $(debugDir)


clean :
	rm -rf $(releaseDir) $(debugDir)





