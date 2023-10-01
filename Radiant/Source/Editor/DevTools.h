#pragma once
#include "Scene/Layer.h"

namespace rdt::core {

	class DevLayer : public Layer {
	private:
	public:

		DevLayer();
		~DevLayer();

		void OnAttach() override final;
		void OnDetach() override final;
	};
}