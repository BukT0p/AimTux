#include "autoblock.h"

bool Settings::Autoblock::enabled = false;
ButtonCode_t Settings::Autoblock::key = ButtonCode_t::KEY_6;

//Thanks teamgamerfood!
void Autoblock::CreateMove(CUserCmd* cmd)
{
	if (!Settings::Autoblock::enabled)
		return;

	if (input->IsButtonDown(Settings::Autoblock::key))
	{
		C_BasePlayer* localplayer = (C_BasePlayer*)entitylist->GetClientEntity(engine->GetLocalPlayer());
		float bestdist = 250.f;
		int index = -1;

		for (int i = 1; i < engine->GetMaxClients(); i++)
		{
			C_BasePlayer* entity = (C_BasePlayer*) entitylist->GetClientEntity(i);

			if (!entity)
				continue;

			if (!entity->GetAlive() || entity->GetDormant() || entity == localplayer)
				continue;

			float dist = localplayer->GetVecOrigin().DistTo(entity->GetVecOrigin());

			if (dist < bestdist)
			{
				bestdist = dist;
				index = i;
			}
		}
 
		if (index == -1)
			return;

		C_BasePlayer* target = (C_BasePlayer*) entitylist->GetClientEntity(index);

		if (!target)
			return;

		QAngle angles = Math::CalcAngle(localplayer->GetVecOrigin(), target->GetVecOrigin());

		angles.y -= localplayer->GetEyeAngles()->y;
		Math::NormalizeAngles(angles);

		if (angles.y < 0.0f)
			cmd->sidemove = 450.f;
		else if (angles.y > 0.0f)
			cmd->sidemove = -450.f;
	}
}
