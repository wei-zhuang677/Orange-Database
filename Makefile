CC = g++
TARGET = Orangedatabase
SOURCE = $(wildcard ./src/*.cpp)      #获取src目录下所有.c文件
OBJS = $(patsubst %.cpp, %.o, $(SOURCE))
INCLUDE = -I./include
$(TARGET):$(OBJS)  
	$(CC) -Iinclude $(OBJS) -o $(TARGET)     
%.o:%.cpp
	$(CC) -Iinclude $(INCLUDE) -c $^ -o $@                                                                                                                                                                        

.PHONY:clean
clean:
	rm $(OBJS) $(TARGET) 
