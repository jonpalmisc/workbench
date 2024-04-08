from binaryninja.log import log_error
from binaryninja import _binaryninjacore as core
from binaryninja.architecture import Architecture
from binaryninja.binaryview import BinaryView
from binaryninja.binaryview import BinaryViewType
from binaryninja.enums import SegmentFlag
from binaryninja import Settings
import json
import traceback

use_default_loader_settings = True


class MappedView(BinaryView):
	name = "STM32F4"
	long_name = "STM32F4"
	load_address = 0x0

	def __init__(self, data):
		BinaryView.__init__(self, parent_view=data, file_metadata=data.file)

	@staticmethod
	def is_valid_for_data(data):
		return True

	@classmethod
	def get_load_settings_for_data(cls, data):
		registered_view = cls.registered_view_type
		assert registered_view is not None
		view = registered_view.parse(data)
		assert view is not None

		load_settings = registered_view.get_default_load_settings_for_data(view)
		load_settings.update_property("loader.imageBase", json.dumps({'default': 0}))
		load_settings.update_property("loader.entryPointOffset", json.dumps({'default': 0}))

		return load_settings

	def init(self):
		try:
			self.arch = Architecture["thumb2"]  # type: ignore
			self.platform = Architecture["thumb2"].standalone_platform  # type: ignore
			self.load_address = 0

			self.add_auto_segment(
			  self.load_address, len(self.parent_view), 0, len(self.parent_view),
			  SegmentFlag.SegmentReadable | SegmentFlag.SegmentExecutable
			)

			self.add_auto_segment(
			  self.load_address, len(self.parent_view), 0, len(self.parent_view),
			  SegmentFlag.SegmentReadable | SegmentFlag.SegmentExecutable
			)

			return True

		except:
			log_error(traceback.format_exc())
			return False

	def perform_get_entry_point(self):
		return 0

	def perform_is_executable(self):
		return True

	def perform_is_relocatable(self):
		return True

	def perform_get_address_size(self):
		return self.arch.address_size


MappedView.register()
