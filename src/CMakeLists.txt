set(rbnc_SOURCES main.cc locator.cc bands.cc icons.cc
    cccluster.cc spottable.cc rbn.cc popup.cc mainwindow.cc settings.cc settingsdialog.cc
	application.cc webfile.cc dxcc.cc menu.cc trayicon.cc colorbutton.cc adi.cc map.cc
  selfspotmap.cc aboutdialog.cc
  clubmembership.cc skcc.cc agcw.cc hsc.cc)
set(rbnc_MOC_HEADERS
    cccluster.hh spottable.hh rbn.hh skcc.hh popup.hh mainwindow.hh settings.hh settingsdialog.hh
	application.hh webfile.hh dxcc.hh menu.hh trayicon.hh colorbutton.hh adi.hh map.hh
  selfspotmap.hh aboutdialog.hh agcw.hh hsc.hh
  clubmembership.hh skcc.hh agcw.hh hsc.hh)
set(rbnc_HEADERS locator.hh bands.hh icons.hh ${skcc_MOC_HEADERS})

configure_file(config.h.in ${PROJECT_BINARY_DIR}/src/config.h)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/src/)

qt5_wrap_cpp(rbnc_MOC_SOURCES ${rbnc_MOC_HEADERS})
qt5_add_resources(rbnc_RCC_SOURCES ../resources.qrc)

add_executable(rbnc WIN32 ${rbnc_SOURCES} ${rbnc_MOC_SOURCES} ${rbnc_RCC_SOURCES})
target_link_libraries(rbnc ${LIBS})

# Create "Application Bundle" under MacOS X
if(UNIX AND APPLE)
 install(TARGETS rbnc DESTINATION /Applications/RBNC.app/Contents/MacOS)
 install(FILES ../dist/macosx/rbnc.icns DESTINATION /Applications/RBNC.app/Contents)
 install(FILES ../dist/macosx/Info.plist DESTINATION /Applications/RBNC.app/Contents)
 install(FILES DESTINATION /Applications/RBNC.app/Contents/Resources)
elseif(UNIX AND NOT APPLE)
  install(TARGETS rbnc DESTINATION bin)
  install(FILES ../dist/linux/rbnc.desktop DESTINATION share/applications/)
  install(FILES ../dist/linux/rbnc.png DESTINATION share/icons/hicolor/64x64/apps/)
endif(UNIX AND APPLE)
