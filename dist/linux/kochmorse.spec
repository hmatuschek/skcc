Summary: A client for the reverse beacon network.

%define version 1.0.0

License: GPL-2.0+
Group: Applications/Communications
Name: rbnc
Prefix: /usr
Release: 1
Source: rbnc-%{version}.tar.gz
URL: https://dm3mat.darc.de/rbn
Version: %{version}
Buildroot: /tmp/rbncrpm
BuildRequires: gcc-c++, cmake
%if 0%{?suse_version}
BuildRequires: libqt5-qtbase-devel, libqt5-qtmultimedia-devel, libqt5-linguist-devel, update-desktop-files
Requires: libqt5-qtbase, libqt5-qtmultimedia, libqt5-linguist
%endif
%if 0%{?fedora}
BuildRequires: qt5-qtbase-devel, qt5-qtmultimedia-devel, qt5-linguist
Requires: qt5-qtbase, qt5-qtmultimedia, qt5-linguist 
%endif

%description
A system-tray client application for the reverse beacon network.


%prep
%setup -q


%build
cmake -DCMAKE_BUILD_TYPE=RELEASE \
      -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT/usr
make


%install
make install
%if 0%{?suse_version}
%suse_update_desktop_file rbnc Education Teaching
%endif

%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-, root, root, -)
%attr(755, root, root) %{_prefix}/bin/rbnc
%{_prefix}/share/applications/rbnc.desktop
%{_prefix}/share/icons/rbnc.png
%{_prefix}/share/io.github.hmatuschek/rbnc/*.png
