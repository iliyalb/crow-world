project('crow-world', 'cpp',
  default_options : ['cpp_std=c++17'])

# Source directories
src = meson.project_source_root() / 'src'
rsc = meson.project_source_root() / 'rsc'
inc = include_directories('include')

# Find dependencies
sqlite_dep = dependency('sqlite3')

# Resources files
install_subdir('rsc', install_dir: meson.current_build_dir(), strip_directory: true)

# Source files
sources = [
  'src/main.cpp',
  'src/post.cpp',
  'src/gallery.cpp',
  'src/html_shared.cpp',
]

# Executable
executable('crow-world.out',
  sources: sources,
  include_directories: inc,
  dependencies: [
    sqlite_dep,
  ])
