#include "ers/ErsSar/ErsSarRecordFactory.h"

namespace ossimplugins
{

ErsSarRecordFactory::ErsSarRecordFactory()
{
}

ErsSarRecordFactory::~ErsSarRecordFactory()
{
}

ErsSarRecord* ErsSarRecordFactory::Instanciate(int id)
{
	ErsSarRecord* record = _availableRecords[id];
	if(record == NULL)
	{
		return NULL;
	}
	else
	{
		return record->Instanciate();
	}
}

void ErsSarRecordFactory::RegisterRecord(int id, ErsSarRecord * record)
{
	_availableRecords[id] = record;
}
}
