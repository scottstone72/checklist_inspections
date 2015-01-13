module.exports = function (grunt) {
  "use strict";

  // Config...
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    // Less config
    less: {
      dev: {
        files: [
          {
            expand: true,     // Enable dynamic expansion.
            cwd: 'src/less/',      // Src matches are relative to this path.
            src: ['*.less','!bootstrap.less','!variables.less','!overrides.less'], // Actual pattern(s) to match.
            dest: 'src/css/',   // Destination path prefix.
            ext: '.css',   // Dest filepaths will have this extension.
            extDot: 'first'   // Extensions in filenames begin after the first dot
          }
        ]
      },
      prod: {
        options: {
          cleancss: true
        },
        files: [
          {
            expand: true,     // Enable dynamic expansion.
            cwd: 'dist/css/',      // Src matches are relative to this path.
            src: ['*.css'], // Actual pattern(s) to match.
            dest: 'dist/css/min/',   // Destination path prefix.
            ext: '.min.css',   // Dest filepaths will have this extension.
            extDot: 'first'   // Extensions in filenames begin after the first dot
          }
        ]
      }
    },
    // vendor-prefix all files
    autoprefixer: {
      dev: {
        expand: true,
        cwd: 'src/css/',
        src: ['*.css'],
        dest: 'dist/css/'
      }
      //prod: {
      //  expand: true,
      //  cwd: 'src/css/',
      //  src: ['*.css'],
      //  dest: 'dist/css/'
      //}
    },
    // Minify all js files
    uglify: {
      //options: {
      //  mangle: false
      //},
      all: {
        files: [
          {
            expand: true,     // Enable dynamic expansion.
            cwd: 'src/js/',      // Src matches are relative to this path.
            src: ['*.js', '!jquery.cookie.js'], // Actual pattern(s) to match.
            dest: 'dist/js/',   // Destination path prefix.
            ext: '.min.js',   // Dest filepaths will have this extension.
            extDot: 'first'   // Extensions in filenames begin after the first dot
          }
        ]
      }
    },
    // Watch for file changes
    watch: {
      options: {
        nospawn: true,
        livereload: true
      },
      files: 'src/less/*.less', // which files to watch
      tasks: ['newer:less:dev','newer:autoprefixer:dev', 'newer:less:prod'],
      scripts: {
        files: ['./**/*.inc', './*.inc', './*.module', './**/*.info', './js/*.js', './js/*.print.js', './src/less/*.less']
      },
      js: {
        files: ['src/js/*.js'],
        tasks: ['newer:uglify:all']
      }
    }

  });

  grunt.loadNpmTasks('grunt-contrib-less');
  grunt.loadNpmTasks('grunt-autoprefixer');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-newer');
  grunt.loadNpmTasks('grunt-contrib-watch');

  // Register our tasks
  grunt.registerTask('devel', ['watch']);
  //grunt.registerTask('prod', ['watch']);

};