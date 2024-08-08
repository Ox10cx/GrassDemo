
#递归将本文件夹下的cpp 全部放入到APP中
file(GLOB_RECURSE FW ./ *.cpp) 

#生成lib库
add_library(fw ${FW})

