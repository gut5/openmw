#ifndef GAME_MWMECHANICS_AIFOLLOW_H
#define GAME_MWMECHANICS_AIFOLLOW_H

#include "typedaipackage.hpp"

#include <string>

#include <components/esm/defs.hpp>

#include "../mwworld/ptr.hpp"

#include "pathfinding.hpp"

namespace ESM
{
namespace AiSequence
{
    struct AiFollow;
}
}

namespace MWMechanics
{
    struct AiFollowStorage : AiTemporaryBase
    {
        float mTimer;
        bool mMoving;
        float mTargetAngleRadians;
        bool mTurnActorToTarget;

        AiFollowStorage() :
            mTimer(0.f),
            mMoving(false),
            mTargetAngleRadians(0.f),
            mTurnActorToTarget(false)
        {}
    };

    /// \brief AiPackage for an actor to follow another actor/the PC
    /** The AI will follow the target until a condition (time, or position) are set. Both can be disabled to cause the actor to follow the other indefinitely
    **/
    class AiFollow final : public TypedAiPackage<AiFollow>
    {
        public:
            AiFollow(const std::string &actorId, float duration, float x, float y, float z);
            AiFollow(const std::string &actorId, const std::string &CellId, float duration, float x, float y, float z);
            /// Follow Actor for duration or until you arrive at a world position
            AiFollow(const MWWorld::Ptr& actor, float duration, float X, float Y, float Z);
            /// Follow Actor for duration or until you arrive at a position in a cell
            AiFollow(const MWWorld::Ptr& actor, const std::string &CellId, float duration, float X, float Y, float Z);
            /// Follow Actor indefinitively
            AiFollow(const MWWorld::Ptr& actor, bool commanded=false);

            AiFollow(const ESM::AiSequence::AiFollow* follow);

            bool sideWithTarget() const final { return true; }
            bool followTargetThroughDoors() const final { return true; }
            bool shouldCancelPreviousAi() const final { return !mCommanded; }

            bool execute (const MWWorld::Ptr& actor, CharacterController& characterController, AiState& state, float duration) final;

            int getTypeId() const final;

            bool useVariableSpeed() const final { return true; }

            /// Returns the actor being followed
            std::string getFollowedActor();

            void writeState (ESM::AiSequence::AiSequence& sequence) const final;

            bool isCommanded() const;

            int getFollowIndex() const;

            void fastForward(const MWWorld::Ptr& actor, AiState& state) final;

            osg::Vec3f getDestination() const final
            {
                MWWorld::Ptr target = getTarget();
                if (target.isEmpty())
                    return osg::Vec3f(0, 0, 0);

                return target.getRefData().getPosition().asVec3();
            }

        private:
            /// This will make the actor always follow.
            /** Thus ignoring mDuration and mX,mY,mZ (used for summoned creatures). **/
            bool mAlwaysFollow;
            bool mCommanded;
            float mDuration; // Hours
            float mRemainingDuration; // Hours
            float mX;
            float mY;
            float mZ;
            std::string mCellId;
            bool mActive; // have we spotted the target?
            int mFollowIndex;

            static int mFollowIndexCounter;
    };
}
#endif
