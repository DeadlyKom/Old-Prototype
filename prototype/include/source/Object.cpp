#include "prototype\include\Object.h"
#include "prototype\engine\include\ObjectRegistry.h"

namespace ScriptEngine
{
	int IObject::GetInstanceID()
	{	
		return instance_id;
	}

	void IObject::Destroy(shared_ptr<IObject> obj)
	{
		SystemEngine::ObjectRegistry::GetInstance()->DeleteObject(obj->GetInstanceID());
	}
}