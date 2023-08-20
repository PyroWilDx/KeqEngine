//
// Created by pyrowildx on 27/07/2023.
//

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "BaseEntity/LivingEntity.hpp"

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

    static BoostPolygon getPolygonFromEntity(Entity *dstEntity);

    bool isHittingEntity(LivingEntity *dstEntity);

    void checkEntityHit(LivingEntity *dstEntity);

    void onGameFrame();

    void renderSelf(SDL_Renderer *gRenderer);

    bool shouldSelfRemove();

    inline void setFollowEntity(Entity *followEntity_) { followEntity = followEntity_; }

    inline void setShouldRemove(bool (*shouldRemove_)(Attack *, void *), void *fParams) {
        shouldRemove = shouldRemove_;
        shouldRemoveParams = fParams;
    }

    inline void setAtkDuration(int atkDuration_) { atkDuration = atkDuration_; }

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

    bool (*shouldRemove)(Attack *, void *);

    void *shouldRemoveParams;

};

#endif
