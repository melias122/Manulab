# TODO(m): found clang-format more sophisticated?
find_program(CLANG_FORMAT_EXE NAMES clang-format)
mark_as_advanced(CLANG_FORMAT_EXE)

function(add_clangformat cf_target)
	if(CLANG_FORMAT_EXE)

		if(NOT TARGET ${cf_target})
			message(FATAL_ERROR "add_clangformat should only be called on targets (got " ${cf_target} ")")
		endif()

		# figure out which sources this should be applied to
		get_target_property(cf_target_sources ${cf_target} SOURCES)

		# get current build directory
		get_target_property(cf_builddir ${cf_target} BINARY_DIR)

		set(cf_stamps "")
		set(cf_cpp_extensions .c .cc .cpp .c++ .cxx .C)

		# we will apply clang-format for every file in target
		foreach(cf_target_source ${cf_target_sources})

			# only for target source files
			if(TARGET ${cf_target_source})
				continue()
			endif()

			# get source file name
			get_filename_component(cf_target_source_name ${cf_target_source} NAME)

			get_filename_component(cf_target_source_name_ext ${cf_target_source_name} EXT)
			if(NOT ${cf_target_source_name_ext} IN_LIST cf_cpp_extensions)
				continue()
			endif()

			# get absolute path to source file
			get_source_file_property(cf_target_source_path "${cf_target_source}" LOCATION)

			# ...
			set(cf_target_source_format_stamp ${cf_target}_${cf_target_source_name}.format)

			# add command to format target file
			add_custom_command(OUTPUT ${cf_target_source_format_stamp}
				DEPENDS ${cf_target_source}
				COMMENT "clang-format ${cf_target_source}"
				COMMAND ${CLANG_FORMAT_EXE} -assume-filename=${CMAKE_SOURCE_DIR}/.clang-format -i ${cf_target_source_path}
				COMMAND ${CMAKE_COMMAND} -E touch ${cf_target_source_format_stamp})

			list(APPEND cf_stamps ${cf_target_source_format_stamp})

		endforeach()

		if(cf_stamps)
			add_custom_target(${cf_target}_clangformat
				SOURCES ${cf_stamps}
				COMMENT "adding clang-format for target ${cf_target}")

			add_dependencies(${cf_target} ${cf_target}_clangformat)
		endif()

	endif()
endfunction()
