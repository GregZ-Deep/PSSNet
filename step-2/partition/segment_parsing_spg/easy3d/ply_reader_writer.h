/*
*	Copyright (C) 2015 by Liangliang Nan (liangliang.nan@gmail.com)
*	https://3d.bk.tudelft.nl/liangliang/
*
*	This file is part of Easy3D: software for processing and rendering
*   meshes and point clouds.
*
*	Easy3D is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License Version 3
*	as published by the Free Software Foundation.
*
*	Easy3D is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef EASY3D_FILEIO_PLY_READER_WRITER_H
#define EASY3D_FILEIO_PLY_READER_WRITER_H


#include <string>
#include <vector>

#include <easy3d/types.h>


namespace easy3d {

	namespace io {

		// VT: the value type, e.g., int, float, string, vec3, std::vector
		template <typename VT>
		class GenericProperty : public std::vector<VT> {
		public:
			GenericProperty(const std::string& elem_name = "", const std::string& prop_name = "", const std::vector<VT>& values = std::vector<VT>())
				: element_name(elem_name)
				, property_name(prop_name)
				, std::vector<VT>(values)
			{}
			std::string property_name;
			std::string element_name;	// i.e., "vertex", "edge", "face"
		};

		typedef GenericProperty<vec3>                   Vec3Property;
		typedef GenericProperty<float>                  FloatProperty;
		typedef GenericProperty<int>                    IntProperty;
		typedef GenericProperty< std::vector<float> >	FloatListProperty;
		typedef GenericProperty< std::vector<int> >     IntListProperty;

		// model element (e.g., faces, vertices, edges) with optional properties
		struct Element {
			Element(const std::string& name, std::size_t n_instances = 0) : element_name(name), num_instances(n_instances) {}
			std::string element_name; // e.g., "vertex", "face", "edge"
			std::size_t num_instances;    // number of instances

			std::vector<Vec3Property>       vec3_properties;    // for "point", "normal", "color", and vector fields
			std::vector<FloatProperty>      float_properties;	// for scalar fields of float values
			std::vector<IntProperty>        int_properties;     // for scalar fields of integer values

			std::vector<FloatListProperty>  float_list_properties;	// for propertis of a list of float values
			std::vector<IntListProperty>	int_list_properties;    // for propertis of a list of integer values
		};

		// A general purpose PLY file writer
		class PlyWriter {
		public:
			bool write(
				const std::string& file_name,
				const std::vector<Element>& elements,
				const std::string& comment = "",
				bool binary = false
			) const;

			static bool is_big_endian();
		};


		// A general purpose PLY file reader
		class PlyReader 
		{
		public:
			bool read(const std::string& file_name, std::vector<Element>& elements);

			// A quick check of the number of faces stored in the ply file (usually
			// used to see if the model is a mesh or a point cloud).
			// Internally it only reads the ply file header.
			static std::size_t num_faces(const std::string& file_name);

            ~PlyReader();

		private:
			// convert the "list" intermediate representation into the user requested format
			void collect_elements(std::vector<Element>& elements) const;

		private:
			// For simpler code, it is possible to save all data as properties of type PLY_LIST
			// with value type double. This can allow us to use a single callback function to 
			// handle all the properties. However, the performance is low. Thus, I process
			// list properties and value properties separately.

			struct PlyProperty { 
				int orig_value_type;	// e.g., PLY_INT, PLY_FLOAT
			};
			struct ListProperty : PlyProperty, GenericProperty< std::vector<double> > {};
			struct ValueProperty : PlyProperty, GenericProperty< double > {};

			std::vector< ListProperty* >	list_properties_;
			std::vector< ValueProperty* >	value_properties_;
		};

	} // namespace io

} // namespace easy3d

#endif

