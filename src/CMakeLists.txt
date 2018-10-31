set(skcc_SOURCES main.cc locator.cc adi.cc bands.cc
    cccluster.cc spottable.cc rbn.cc skcc.cc popup.cc mainwindow.cc settings.cc settingsdialog.cc
	application.cc webfile.cc dxcc.cc)
set(skcc_MOC_HEADERS
    cccluster.hh spottable.hh rbn.hh skcc.hh popup.hh mainwindow.hh settings.hh settingsdialog.hh
	application.hh webfile.hh dxcc.hh )
set(skcc_HEADERS locator.hh adi.hh bands.hh ${skcc_MOC_HEADERS})

qt5_wrap_cpp(skcc_MOC_SOURCES ${skcc_MOC_HEADERS})
qt5_add_resources(skcc_RCC_SOURCES ../resources.qrc)

add_executable(skcc WIN32 ${skcc_SOURCES} ${skcc_MOC_SOURCES} ${skcc_RCC_SOURCES})
target_link_libraries(skcc ${LIBS})

set(dxcc_SOURCES dxcc_test.cc
	dxcc.cc webfile.cc)
set(dxcc_MOC_HEADERS
	dxcc.hh webfile.hh)
#qt5_wrap_cpp(dxcc_MOC_SOURCES ${dxcc_MOC_HEADERS})
#add_executable(dxcc WIN32 ${dxcc_SOURCES} ${dxcc_MOC_SOURCES})
#target_link_libraries(dxcc ${LIBS})

install(TARGETS skcc DESTINATION bin)

# Create "Application Bundle" under MacOS X
if(UNIX AND APPLE)
 install(TARGETS skcc DESTINATION /Applications/SKCC.app/Contents/MacOS)
 install(FILES ../dist/macosx/skcc.icns DESTINATION /Applications/SKCC.app/Contents)
 install(FILES ../dist/macosx/Info.plist DESTINATION /Applications/SKCC.app/Contents)
endif(UNIX AND APPLE)
