//
// Created by pyrowildx on 27/07/2023.
//

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

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
           int damage, double kbXVelocity, double kbYVelocity);

    Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
           int damage, double kbXVelocity, double kbYVelocity);

    ~Attack();

    void setClassicParticle(int n, bool electro);

    static BoostPolygon getPolygonFromEntity(Entity *dstEntity);

    bool isHittingEntity(LivingEntity *dstEntity);

    void checkEntityHit(LivingEntity *dstEntity);

    void onGameFrame();

    void renderSelf(SDL_Renderer *gRenderer);

    bool shouldSelfRemove();

    inline void setFollowEntity(Entity *followEntity_) { followEntity = followEntity_; }

    inline void setKbXVelocity(double kbXVelocity_) { kbXVelocity = kbXVelocity_; }

    inline void setKbYVelocity(double kbYVelocity_) { kbYVelocity = kbYVelocity_; }

    inline void setBigParticle(Particle *bigParticle_) { bigParticle = bigParticle_; }

    inline void setSmallParticle(Particle *smallParticle_) { smallParticle = smallParticle_; }

    inline void setUniqueEntityHit(bool uniqueEntityHit_) { uniqueEntityHit = uniqueEntityHit_; }

    inline void setOnHit(void (*onHit_)(Attack *, LivingEntity *, void *), void *fParams) {
        onHit = onHit_;
        onHitParams = fParams;
    }

    inline void setShouldRemove(bool (*shouldRemove_)(Attack *, void *), void *fParams) {
        shouldRemove = shouldRemove_;
        shouldRemoveParams = fParams;
    }

    inline void setAtkDuration(int atkDuration_) { atkDuration = atkDuration_; }

    inline LivingEntity *getAtkIssuer() { return atkIssuer; }

private:
    LivingEntity *atkIssuer;
    Entity *followEntity;
    double followEntityLastX, followEntityLastY;
    BoostPolygon atkPolygon;
    int damage;
    double kbXVelocity;
    double kbYVelocity;
    int atkTimeAcc;
    int atkDuration;
    Particle *bigParticle;
    Particle *smallParticle;
    bool uniqueEntityHit;
    std::vector<LivingEntity *> hitEntityVector;

    void (*onHit)(Attack *, LivingEntity *, void *);

    void *onHitParams;

    bool (*shouldRemove)(Attack *, void *);

    void *shouldRemoveParams;

};

#endif
