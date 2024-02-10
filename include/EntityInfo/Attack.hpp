//
// Created by pyrowildx on 27/07/2023.
//

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <string>
#include <unordered_set>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "Entity/DamageText.hpp"
#include "Utils/Colors.hpp"

#define ATK_SMALL_PARTICLE_COUNT 12

class Entity;

class LivingEntity;

class Particle;

namespace bst_geo = boost::geometry;

typedef bst_geo::model::d2::point_xy<double> BoostPoint;
typedef bst_geo::model::polygon<BoostPoint> BoostPolygon;

class Attack {

public:
    Attack(LivingEntity *atkIssuer_, Entity *followEntity,
           double xyArray[][2], int arrayLength,
           int atkDamage, double kbXVelocity, double kbYVelocity);

    Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
           int atkDamage, double kbXVelocity, double kbYVelocity);

    ~Attack();

    void setHitSound(const char *fileName);

    void setHitSound(std::string &fileName);

    void setKQHitSoundRandom(int atkStrength);

    void setElementalAtkTrue();

    void setClassicParticle(int n, bool atkElectro);

    static BoostPolygon getPolygonFromEntity(Entity *dstEntity);

    BoostPoint isHittingEntity(LivingEntity *dstEntity);

    void checkEntityHit(LivingEntity *dstEntity);

    bool haveHitEntity(LivingEntity *checkEntity);

    void onGameFrame();

    void renderSelf(SDL_Renderer *gRenderer);

    bool shouldSelfRemove();

    inline void setFollowEntity(Entity *followEntity_) { followEntity = followEntity_; }

    inline void setAtkDamage(int atkDamage_) { atkDamage = atkDamage_; }

    inline void setKbXVelocity(double kbXVelocity_) { kbXVelocity = kbXVelocity_; }

    inline void setKbYVelocity(double kbYVelocity_) { kbYVelocity = kbYVelocity_; }

    inline void setBigParticle(Particle *bigParticle_) { bigParticle = bigParticle_; }

    inline void setSmallParticle(Particle *smallParticle_) { smallParticle = smallParticle_; }

    inline void setUniqueEntityHit(bool uniqueEntityHit_) { uniqueEntityHit = uniqueEntityHit_; }

    inline void setDmgTextSettings(SDL_Color *dmgTextColor_, int dmgTextFontSize_,
                                   int dmgTextDuration_, double dmgTextYOffset_) {
        if (dmgTextColor_ != nullptr) dmgTextColor = dmgTextColor_;
        if (dmgTextFontSize_ != 0) dmgTextFontSize = dmgTextFontSize_;
        if (dmgTextDuration_ != 0) dmgTextDuration = dmgTextDuration_;
        if (dmgTextYOffset_ != 0) dmgTextYOffset = dmgTextYOffset_;
    }

    inline void setOnHit(void (*onHit_)(Attack *, LivingEntity *, void *), void *fParams) {
        onHit = onHit_;
        onHitParams = fParams;
    }

    inline void setShouldRemove(bool (*shouldRemove_)(Attack *, void *), void *fParams) {
        shouldRemove = shouldRemove_;
        shouldRemoveParams = fParams;
    }

    inline void setAtkDuration(int atkDuration_) { atkDuration = atkDuration_; }

    inline void setAtkCompanion(Attack *atkCompanion_) {
        atkCompanion = atkCompanion_;
        atkCompanion_->atkCompanion = this;
    }

    [[nodiscard]] inline LivingEntity *getAtkIssuer() const { return atkIssuer; }

    [[nodiscard]] inline bool getIsElemental() const { return isElemental; }

private:
    LivingEntity *atkIssuer;
    Entity *followEntity;
    double followEntityLastX, followEntityLastY;
    BoostPolygon atkPolygon;
    int atkDamage;
    double kbXVelocity;
    double kbYVelocity;
    int atkTimeAcc;
    int atkDuration;
    std::string hitSoundPath;
    bool isElemental;
    Particle *bigParticle;
    Particle *smallParticle;
    bool uniqueEntityHit;
    std::unordered_set<LivingEntity *> hitEntitySet;
    Attack *atkCompanion;
    SDL_Color *dmgTextColor;
    int dmgTextFontSize;
    int dmgTextDuration;
    double dmgTextYOffset;

    void (*onHit)(Attack *, LivingEntity *, void *);

    void *onHitParams;

    bool (*shouldRemove)(Attack *, void *);

    void *shouldRemoveParams;

};

#endif
