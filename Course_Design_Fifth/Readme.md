- [ ] Course_Design_Fifth 文件为Clion项目文件，该项目文件在Clion2020.3版本下可以正常运行；

  Course_Design_Fifth 项目中的txt等文件存放在cmake-build-debug目录下。

  ------

  

- [ ] Fifth.cpp文件为代码源文件，其在x86_64-8.1.0-release-posix-sjlj-rt_v6-rev0版本下的mingw64编译器下可以正常编译运行，但是其在运行时需要正确的环境配置，环境配置不正确可能出现 “无法定位程序输入_gxx_personality_v0”的运行错误，其解决方案如下：

    1.使用-static 编译[g++ -static file.cpp]

    2.删除掉其他含有libstdc++-6.dll 的PATH环境变量，只留下mingw的

    3.将 libstdc++-6.dll 文件与代码放置同一文件编译

    4.将 libstdc++-6.dll 文件放到System32或SysWOW64文件夹下。

  ------

  

- [ ] 如果libstdc++-6.dll 文件放置到相应的位置后出现 “0xc000007b——应用程序无法正常启动”，则表明libstdc++-6.dll 文件的版本错误，需要更换正确的版本。

  ------

- [ ] 为使程序能够正确运行，程序提供了两个入口，即命令行或直接运行，可根据实际需要选择：

  ```c++
  /*-------------------Main函数-----------------------*/
  /*int main(int argc,char **argv) {
      UpdataMap();
      if(strcmp(argv[1],"-a")==0){
          Manager manager;
          if(manager.Login_Judge(argv[2],1)){
              manager.Search_Menu();
          }
      } else if (strcmp(argv[1],"-u")==0){
          Reader reader;
          if(reader.Login_Judge(argv[2],2)){
              reader.Search_Menu();
          }
      }
      return 0;
  }*/
  int main(){
      UpdataMap();
      string a,b;
      cin>>a>>b;
      if(a=="-a"){
          Manager manager;
          if(manager.Login_Judge(b,1)){
              manager.Search_Menu();
          }
      } else if (a=="-u"){
          Reader reader;
          if(reader.Login_Judge(b,2)){
              reader.Search_Menu();
          }
      }
      return 0;
  }
  ```

  

- [ ] <u>**总结：**</u>**直接在clion环境下运行该程序是最简单的方法，不需要环境的适配，采用其他IDE可能需要正确的运行环境。**