project(
  'wayne',
  'c',
  version: '0.0.1',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

cc = meson.get_compiler('c')

add_project_arguments(
  [
    '-DWLR_USE_UNSTABLE',
    '-D_POSIX_C_SOURCE=200809L'
  ],
  language: 'c',
)

wayland_protocols = dependency('wayland-protocols')
rt = cc.find_library('rt')

deps = [
  rt,
  dependency('wlroots-0.19'),
  dependency('xkbcommon'),
  dependency('wayland-server'),
  wayland_protocols,
  dependency('pixman-1')
]

subdir('protocols')

executable(
  meson.project_name(),
  files(
    'src/input.c',
    'src/output.c',
    'src/layer.c',
    'src/xdg.c',
    'src/util.c',
    'src/main.c'
  ),
  dependencies: deps
)
