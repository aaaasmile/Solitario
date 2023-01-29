Summary: Solitario
Name: solitario
Version: 1.0
Release: 1
Source: %{name}-%{version}.tar.gz
Url: https://invido.it
Copyright: GPL
Group: Svaghi/Giochi
Vendor: Invido.it <6colpiunbucosolo@gmx.net>
Packager: Fabio Spataro <fabio.spataro@fis.unipr.it>
Prefix: %{_prefix}
BuildRoot: %{_tmppath}/%{name}-root

%description
Questo è il gioco Solitario conn le carte da briscola. I mazzi supportati sono
14. Tra le lingue del programma vi è anche il dialetto bredese.

%prep
%setup

%build
./configure --prefix=%{_prefix}
make

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc AUTHORS README
%{_bindir}/*
%{_datadir}/%{name}/*

%changelog
* Thu May 06 2004 Fabio Spataro <fabio.spataro@fis.unipr.it>
- Versione 0.1
