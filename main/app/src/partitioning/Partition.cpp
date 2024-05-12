#include "Partition.hpp"

void Partition::addObject(SceneObject* object) {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				if (clusters[i][j][k]->containsObject(object))
					clusters[i][j][k]->addObject(object);
			}
		}
	}
}

void Partition::initializeCluserNeighbours() {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				std::vector<Cluster*> neighbours;

				correctIndex(i + 1, width_count) ? neighbours.push_back(clusters[i + 1][j][k]) : void();
				correctIndex(i - 1, width_count) ? neighbours.push_back(clusters[i - 1][j][k]) : void();
				correctIndex(j + 1, height_count) ? neighbours.push_back(clusters[i][j + 1][k]) : void();
				correctIndex(j - 1, height_count) ? neighbours.push_back(clusters[i][j - 1][k]) : void();
				correctIndex(k + 1, length_count) ? neighbours.push_back(clusters[i][j][k + 1]) : void();
				correctIndex(k - 1, length_count) ? neighbours.push_back(clusters[i][j][k - 1]) : void();

				for (Cluster* neighbour : neighbours)
					clusters[i][j][k]->addNeighbour(neighbour);
			}
		}
	}
}

void Partition::initializeClusters() {
	float edge_length = size / (float)(level);

	for (int i = 0; i < width_count; ++i) {
		for (int j = 0; j < height_count; ++j) {
			for (int k = 0; k < length_count; ++k) {
				float x = i * edge_length;
				float y = j * edge_length;
				float z = k * edge_length;

				glm::vec3 translation = glm::vec3(x, y, z);
				float e = edge_length / 2.0f;
				translation += glm::vec3(glm::vec3(e, 0.0f, e));
				float p = size / 2.0f;
				translation += glm::vec3(-p, 0.0f, -p);

				Cluster* cluster = new Cluster(edge_length);
				cluster->translate(translation);
				clusters[i][j][k] = cluster;
			}
		}
	}
}

void Partition::emptyClusters() {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->emptyCluster();
			}
		}
	}
		
}

void Partition::update() {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->setObjectsUpdatedValue(false);
			}
		}
	}

	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->updateObjectInteractions();
			}
		}
	}

	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->updateObjectTransformations();
			}
		}
	}

	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->updateClusterContent();
			}
		}
	}
}

void Partition::draw(const Camera& camera) const {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				clusters[i][j][k]->drawFrame(camera);
			}
		}
	}
}

Partition::~Partition() {
	for (int i = 0; i < width_count; i++) {
		for (int j = 0; j < height_count; j++) {
			for (int k = 0; k < length_count; k++) {
				delete clusters[i][j][k];
			}
		}
	}
}
