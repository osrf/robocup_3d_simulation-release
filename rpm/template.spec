Name:           ros-indigo-robocup-msgs
Version:        0.0.6
Release:        1%{?dist}
Summary:        ROS robocup_msgs package

Group:          Development/Libraries
License:        Apache 2.0
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-indigo-geometry-msgs
Requires:       ros-indigo-message-runtime
Requires:       ros-indigo-sensor-msgs
Requires:       ros-indigo-std-msgs
Requires:       ros-indigo-trajectory-msgs
BuildRequires:  ros-indigo-catkin
BuildRequires:  ros-indigo-geometry-msgs
BuildRequires:  ros-indigo-message-generation
BuildRequires:  ros-indigo-sensor-msgs
BuildRequires:  ros-indigo-std-msgs
BuildRequires:  ros-indigo-trajectory-msgs

%description
The robocup_msgs package This package provides the custom messages for
robocup_3d_simulation. Other standard ROS messages are also used.

%prep
%setup -q

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/indigo/setup.sh" ]; then . "/opt/ros/indigo/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/indigo" \
        -DCMAKE_PREFIX_PATH="/opt/ros/indigo" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/indigo/setup.sh" ]; then . "/opt/ros/indigo/setup.sh"; fi
cd obj-%{_target_platform}
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/indigo

%changelog
* Fri Jan 16 2015 Tully Foote <tfoote@osrfoundation.org> - 0.0.6-1
- Autogenerated by Bloom

* Mon Dec 15 2014 Tully Foote <tfoote@osrfoundation.org> - 0.0.6-0
- Autogenerated by Bloom

* Thu Dec 04 2014 Tully Foote <tfoote@osrfoundation.org> - 0.0.5-0
- Autogenerated by Bloom

* Wed Dec 03 2014 Tully Foote <tfoote@osrfoundation.org> - 0.0.3-0
- Autogenerated by Bloom

* Wed Dec 03 2014 Tully Foote <tfoote@osrfoundation.org> - 0.0.4-0
- Autogenerated by Bloom

* Mon Dec 01 2014 Tully Foote <tfoote@osrfoundation.org> - 0.0.2-0
- Autogenerated by Bloom

