//
// Created by pyrow on 27/07/2023.
//

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "EntityBase/LivingEntity.hpp"

namespace bst_geo = boost::geometry;

typedef bst_geo::model::d2::point_xy<double> BoostPoint;
typedef bst_geo::model::polygon<BoostPoint> BoostPolygon;

class Attack {

public:
    Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
           int damage, double kbXVelocity, double kbYVelocity);

    static BoostPolygon *getPolygonFromEntity(Entity *dstEntity);

    bool isHittingEntity(LivingEntity *dstEntity);

    void checkEntityHit(LivingEntity *dstEntity);

    void onGameFrame();

    void renderSelf(SDL_Renderer *gRenderer);

    bool shouldSelfRemove();

    inline void setAtkIssuerDependant(bool atkIssuerDependant_) { atkIssuerDependant = atkIssuerDependant_; }

    inline void setShouldRemove(bool (*shouldRemove_)(Attack *self, void *fParams), void *fParams) {
        shouldRemove = shouldRemove_;
        shouldRemoveParams = fParams;
    }

    inline void setAtkDuration(int atkDuration_) { atkDuration = atkDuration_; }

private:
    LivingEntity *atkIssuer;
    bool atkIssuerDependant;
    double atkIssuerLastX, atkIssuerLastY;
    BoostPolygon atkPolygon;
    int damage;
    double kbXVelocity;
    double kbYVelocity;
    int atkTimeAcc;
    int atkDuration;

    bool (*shouldRemove)(Attack *self, void *fParams);

    void *shouldRemoveParams;

};

#endif
