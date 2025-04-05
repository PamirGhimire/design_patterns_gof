/**
--------------------
PROBLEM 
--------------------
🔧 Scenario: Configurable Firmware Package Generator
You’re designing a tool that creates firmware packages for embedded devices. Each firmware package includes:
A bootloader
A main application
An optional diagnostics module
An optional debug log
A version string
Some firmware packages are minimal (just bootloader + app), others are full-featured with diagnostics and debug logs.

✅ Requirements:
You must be able to build different configurations of a firmware package.
The construction process should be step-by-step, but the final object should be assembled cleanly.
The client code should not need to know the internal details of how each piece is put together.

🧠 Your Task:
Design a solution using the Builder pattern.
Think about:
What your FirmwarePackage class looks like.
What the FirmwareBuilder interface defines.
How a MinimalFirmwareBuilder differs from a FullFeatureFirmwareBuilder.
How a FirmwareDirector might guide the build process.
**/


#include <iostream>
#include <memory>

class Bootloader{};
class MainApp{};
class DiagModule{};
class DebugLog{};
class VersionString{};
class FirmwarePackage
{
  public:
  void setBootloader(std::unique_ptr<Bootloader> bootloader) {bootloader_ = std::move(bootloader);}
  void setMainApp(std::unique_ptr<MainApp> mainApp) {mainApp_ = std::move(mainApp);}
  void setDiagModule(std::unique_ptr<DiagModule> diagModule){diagModule_ = std::move(diagModule);}
  void setDebugLog(std::unique_ptr<DebugLog> debugLog){debugLog_ = std::move(debugLog);}
  void setVersionString(std::unique_ptr<VersionString> versionString){versionString_ = std::move(versionString);}

  void printStatus() const
  {
      if (bootloader_){std::cout << "has bootloader" << std::endl;}
      if (mainApp_){std::cout << "has main app" << std::endl;}
      if (diagModule_){std::cout << "has diag module" << std::endl;}
      if (debugLog_){std::cout << "has debug log" << std::endl;}
      if (versionString_){std::cout << "has version string" << std::endl;}
  }

  private:
  std::unique_ptr<Bootloader> bootloader_{nullptr};
  std::unique_ptr<MainApp> mainApp_{nullptr};
  std::unique_ptr<DiagModule> diagModule_{nullptr};
  std::unique_ptr<DebugLog> debugLog_{nullptr};
  std::unique_ptr<VersionString> versionString_{nullptr};
};

class AbstractFirmwareBuilder
{
  public:
  virtual AbstractFirmwareBuilder& BuildBootloader() = 0;
  virtual AbstractFirmwareBuilder& BuildMainApp() = 0;
  virtual AbstractFirmwareBuilder& BuildDiagModule() = 0;
  virtual AbstractFirmwareBuilder& BuildDebugLog() = 0;
  virtual AbstractFirmwareBuilder& BuildVersionString() = 0;
  virtual FirmwarePackage& GetFirmwarePackage() = 0;
  virtual ~AbstractFirmwareBuilder() = default;
};


class FirmwareBuilder: public AbstractFirmwareBuilder
{
  public:
  FirmwarePackage& GetFirmwarePackage() override {return firmwarePackage_;};

  public:
  FirmwareBuilder& BuildBootloader() override {
      firmwarePackage_.setBootloader(std::make_unique<Bootloader>());
      return *this;
  };
  FirmwareBuilder& BuildMainApp() override {
      firmwarePackage_.setMainApp(std::make_unique<MainApp>());
      return *this;
  };
  FirmwareBuilder& BuildDiagModule() override {
      firmwarePackage_.setDiagModule(std::make_unique<DiagModule>());
      return *this;
  };
  FirmwareBuilder& BuildDebugLog() override {
      firmwarePackage_.setDebugLog(std::make_unique<DebugLog>());
      return *this;
  };
  FirmwareBuilder& BuildVersionString() override {
      firmwarePackage_.setVersionString(std::make_unique<VersionString>());
      return *this;
  };
  
  private:
  FirmwarePackage firmwarePackage_;
};

class FirmwareBuilderWithoutDiag : public FirmwareBuilder
{
  public:
  FirmwareBuilder& BuildDiagModule() override {
      return *this;
  };
};

class Director{
  public:
  Director(AbstractFirmwareBuilder& firmwareBuilder)
  : firmwareBuilder_(firmwareBuilder){}

  void Construct(){
    firmwareBuilder_.
        BuildBootloader().
        BuildMainApp().
        BuildDiagModule().
        BuildDebugLog().
        BuildVersionString();
  };
  
  
  private:
  AbstractFirmwareBuilder& firmwareBuilder_;
};


// client
int main() {
    FirmwareBuilderWithoutDiag firmwareBuilder;
    Director director(firmwareBuilder);
    director.Construct();
    FirmwarePackage& firmwarePackage = firmwareBuilder.GetFirmwarePackage();
    firmwarePackage.printStatus();

    return 0;
}

