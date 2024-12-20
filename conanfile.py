from conans import ConanFile, CMake

class http_server_recipe(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "cmake_find_package"
    
    
    def configure(self):
        self.options['poco'].enable_jwt = False
        self.options['poco'].enable_zip = False
        self.options['poco'].enable_redis = False
        self.options['poco'].enable_crypto = False
        self.options['poco'].enable_netssl = False
        self.options['poco'].enable_mongodb = False
        self.options['poco'].enable_encodings = False
        self.options['poco'].enable_data_mysql = False
        self.options['poco'].enable_data_sqlite = False
        self.options['poco'].enable_json = False
    
    
    def requirements(self):
        self.requires("poco/1.13.3")
        self.requires("nlohmann_json/3.11.3")
    
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()