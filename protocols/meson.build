wl_protocol_dir = wayland_protocols.get_variable('pkgdatadir')

wayland_scanner_dep = dependency('wayland-scanner', native: true)
wayland_scanner = find_program(
	wayland_scanner_dep.get_variable('wayland_scanner'),
	native: true,
)

protocols = [
	wl_protocol_dir / 'stable/xdg-shell/xdg-shell.xml',
	'wlr-layer-shell-unstable-v1.xml',
]

wl_protocols_src = []

foreach xml : protocols
	wl_protocols_src += custom_target(
		xml.underscorify() + '_server_h',
		input: xml,
		output: '@BASENAME@-protocol.h',
		command: [wayland_scanner, 'server-header', '@INPUT@', '@OUTPUT@'],
	)
endforeach
