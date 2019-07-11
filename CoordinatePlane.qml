import Qt3D.Core 2.0
import Qt3D.Render 2.12
import Qt3D.Extras 2.0

Entity {
    id: root
    property Layer layer: null

    Transform {
        id: planeTransform
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
        translation: Qt.vector3d(0.5, 0.5, -0.01)
    }

    PhongAlphaMaterial {
        id: planeMaterial
        alpha: 0.6
        ambient: "red"
        diffuse: "black"
        shininess: 0.5
        specular: "gray"
    }

    PlaneMesh {
        id: planeMesh
        height: 1.0
        width: 1.0
        //meshResolution: Qt.size(100, 100)
    }

    components: [ planeTransform, planeMesh, planeMaterial, layer ]
}
