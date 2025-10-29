project('crow-world', 'cpp',
  default_options : ['cpp_std=c++17'])

# Source directories
src = meson.project_source_root() / 'src'
rsc = meson.project_source_root() / 'rsc'
db = meson.project_source_root() / 'db'
inc = include_directories('include')

# Find dependencies
sqlite_dep = dependency('sqlite3')

# Source files
sources = [
  'src/main.cpp',
  'src/post.cpp',
  'src/gallery.cpp',
  'src/html_shared.cpp',
]

# Resources files
install_subdir('rsc', install_dir: meson.current_build_dir(), strip_directory: true)

# Database storage
configure_file(
  input: db / 'storage.db',
  output: 'storage.db',
  copy: true
)

# Executable
executable('crow-world.out',
  sources: sources,
  include_directories: inc,
  dependencies: [
    sqlite_dep,
  ])
