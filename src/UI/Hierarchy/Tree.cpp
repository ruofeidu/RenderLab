#include <UI/Tree.h>

#include <UI/Hierarchy.h>
#include <UI/Attribute.h>

#include <CppUtil/Basic/GStorage.h>
#include <CppUtil/Engine/SObj.h>
#include <CppUtil/Engine/TriMesh.h>
#include <CppUtil/Engine/AllComponents.h>
#include <CppUtil/Engine/AllBSDFs.h>
#include <CppUtil/Engine/Sphere.h>
#include <CppUtil/Engine/Plane.h>
#include <CppUtil/Engine/AreaLight.h>
#include <CppUtil/Engine/PointLight.h>

#include <qmenu.h>
#include <qdrag.h>
#include <qevent.h>
#include <qmimedata.h>
#include <qfiledialog.h>

using namespace Ui;
using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace glm;
using namespace std;

Tree::Tree(QWidget *parent)
	: QTreeWidget(parent), needDragItem(nullptr), canDrag(false) {

	setAcceptDrops(true);

	connect(this, &QTreeWidget::currentItemChanged, this, &Tree::on_tree_Hierarchy_currentItemChanged);
	connect(this, &QTreeWidget::itemClicked, this, &Tree::on_tree_Hierarchy_itemClicked);
	connect(this, &QTreeWidget::itemPressed, this, &Tree::on_tree_Hierarchy_itemPressed);

}

void Tree::dragEnterEvent(QDragEnterEvent *event) {
	//printf("drag enter\n");
	event->setDropAction(Qt::MoveAction);
	event->accept();
	//QTreeWidget::dragEnterEvent(event);
}

void Tree::dragLeaveEvent(QDragLeaveEvent *event) {
	//printf("drag leave\n");
	event->accept();
	//QTreeWidget::dragLeaveEvent(event);
}

void Tree::dragMoveEvent(QDragMoveEvent *event) {
	//printf("drag move\n");
	event->setDropAction(Qt::MoveAction);
	event->accept();
	auto mE = new QMouseEvent(QEvent::MouseMove, event->posF(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	mouseMoveEvent(mE);
	delete mE;
	//QTreeWidget::dragMoveEvent(event);
}

void Tree::dropEvent(QDropEvent *event) {
	//printf("drop\n");
	event->setDropAction(Qt::MoveAction);
	event->accept();
	
	string key = event->mimeData()->text().toStdString();
	//printf("text: %s\n", key.c_str());

	QTreeWidgetItem * item;
	GS::GetV(key, item);
	GS::UnReg<QTreeWidgetItem *>(key);

	Hierarchy::GetInstance()->Move(item, currentItem());
	//QTreeWidget::dropEvent(event);
}


void Tree::on_tree_Hierarchy_itemClicked(QTreeWidgetItem *item, int column) {
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
#ifndef NDEBUG
	printf("click: %s\n", sobj->name.c_str());
#endif // !NDEBUG
	Attribute::GetInstance()->SetSObj(sobj);
}

void Tree::on_tree_Hierarchy_itemPressed(QTreeWidgetItem *item, int column) {
	if (canDrag)
		needDragItem = item;

#ifndef NDEBUG
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
	printf("press: %s\n", sobj->name.c_str());
#endif // !NDEBUG
}

void Tree::on_tree_Hierarchy_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
#ifndef NDEBUG
	auto psobj = Hierarchy::GetInstance()->GetSObj(previous);
	auto csobj = Hierarchy::GetInstance()->GetSObj(current);
	printf("cur item change: [p]%s, [c]%s\n", psobj ? psobj->name.c_str() : "empty", csobj->name.c_str());
#endif // !NDEBUG

	if (needDragItem && current != previous) {
		auto drag = new QDrag(this);

		auto mimedata = new QMimeData;
		GS::Reg("dragItem", needDragItem);
		needDragItem = nullptr;
		mimedata->setText("dragItem");
		drag->setMimeData(mimedata);

		drag->exec();

#ifndef NDEBUG
		printf("drag return\n");
#endif // !NDEBUG
	}
}

void Tree::on_tree_Hierarchy_itemChanged(QTreeWidgetItem *item, int column) {
#ifndef NDEBUG
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
	printf("item change: %s\n", sobj ? sobj->name.c_str() : "empty");
#endif // !NDBUG
}

void Tree::on_tree_Hierarchy_itemEntered(QTreeWidgetItem *item, int column) {
#ifndef NDEBUG
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
	auto items = selectedItems();
	auto selectedSObj = Hierarchy::GetInstance()->GetSObj(*items.begin());
	printf("enter: %s, cur selection: %d, [0]%s\n", sobj->name.c_str(), items.size(), selectedSObj->name.c_str());
#endif // !NDEBUG
}

void Tree::on_tree_Hierarchy_itemActivated(QTreeWidgetItem *item, int column) {
#ifndef NDEBUG
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
	printf("activate: %s\n", sobj->name.c_str());
#endif // !NDEBUG
}

void Tree::on_tree_Hierarchy_itemDoubleClicked(QTreeWidgetItem *item, int column) {
#ifndef NDEBUG
	auto sobj = Hierarchy::GetInstance()->GetSObj(item);
	printf("double click: %s\n", sobj->name.c_str());
#endif // !NDEBUG
}

void Tree::on_tree_Hierarchy_itemSelectionChanged() {
#ifndef NDEBUG
	auto items = selectedItems();
	auto selectedSObj = Hierarchy::GetInstance()->GetSObj(*items.begin());
	printf("selection change: %d, [0]%s\n", items.size(), selectedSObj->name.c_str());
#endif // !NDEBUG
}

void Tree::mousePressEvent(QMouseEvent *event){
	canDrag = event->button() == Qt::LeftButton;
	needDragItem = nullptr;
	QTreeWidget::mousePressEvent(event);
}

void Tree::mouseReleaseEvent(QMouseEvent *event){
	canDrag = false;
	needDragItem = nullptr;
	QTreeWidget::mouseReleaseEvent(event);
}

void Tree::contextMenuEvent(QContextMenuEvent *event) {
	QMenu mainMenu;

	if (currentItem()) {
		mainMenu.addAction("Rename", this, [=]() {
			Hierarchy::GetInstance()->RenameCurItem();
		});
	}

	if (currentItem() && Hierarchy::GetInstance()->GetSObj(currentItem()) != Hierarchy::GetInstance()->GetRoot()) {
		mainMenu.addAction("Delete", this, [=]() {
			Hierarchy::GetInstance()->DeleteSObj();
		});
	}

	auto spitLine0 = new QAction;
	spitLine0->setSeparator(true);
	mainMenu.addAction(spitLine0);

	mainMenu.addAction("Create Empty", this, [=]() {
		Hierarchy::GetInstance()->CreateSObj("SObj");
	});

	// gen obj
	auto genObjMenu = new QMenu;
	genObjMenu->setTitle("Create 3D Object");

	genObjMenu->addAction("Cube", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Cube");
		auto transform = ToPtr(new Transform(sobj));
		auto geometry = ToPtr(new Geometry(sobj, TriMesh::GenCube()));
		auto material = ToPtr(new Material(sobj, ToPtr(new BSDF_Diffuse)));
	});

	genObjMenu->addAction("Sphere", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Sphere");
		auto transform = ToPtr(new Transform(sobj));
		auto geometry = ToPtr(new Geometry(sobj, ToPtr(new Sphere)));
		auto material = ToPtr(new Material(sobj, ToPtr(new BSDF_Diffuse)));
	});

	genObjMenu->addAction("Plane", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Plane");
		auto transform = ToPtr(new Transform(sobj));
		auto geometry = ToPtr(new Geometry(sobj, ToPtr(new Plane)));
		auto material = ToPtr(new Material(sobj, ToPtr(new BSDF_Diffuse)));
	});

	mainMenu.addMenu(genObjMenu);

	// light
	auto genLightMenu = new QMenu;
	genLightMenu->setTitle("Create Light");

	genLightMenu->addAction("Area Light", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Area Light");
		auto areaLight = ToPtr(new AreaLight);
		auto light = ToPtr(new Light(sobj, areaLight));
		auto lightTransform = ToPtr(new Transform(sobj));
		auto lightPlane = ToPtr(new Plane);
		auto lightGeo = ToPtr(new Geometry(sobj, lightPlane));
	});

	genLightMenu->addAction("Point Light", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Point Light");
		auto light = ToPtr(new Light(sobj, ToPtr(new PointLight)));
		auto transform = ToPtr(new Transform(sobj));
	});

	mainMenu.addMenu(genLightMenu);

	mainMenu.addAction("Create Camera", this, [=]() {
		auto sobj = Hierarchy::GetInstance()->CreateSObj("Camera");
		auto transform = ToPtr(new Transform(sobj));
		auto camera = ToPtr(new Camera(sobj));
	});

	auto spitLine1 = new QAction;
	spitLine1->setSeparator(true);
	mainMenu.addAction(spitLine1);

	// Load Save
	mainMenu.addAction("Load SObj", this, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this,
			tr("Load SObj"),
			"./",
			tr("SObj XML Files (*.xml)"));

		if (fileName.isEmpty())
			return;

		auto sobj = SObj::Load(fileName.toStdString());
		if (sobj == nullptr)
			return;

		Hierarchy::GetInstance()->BindSObj(sobj);
	});

	if (currentItem()) {
		mainMenu.addAction("Save SObj", this, [=]() {
			QString fileName = QFileDialog::getSaveFileName(this,
				tr("Save SObj"),
				"./",
				tr("SObj XML Files (*.xml)"));

			if (fileName.isEmpty())
				return;

			auto sobj = Hierarchy::GetInstance()->GetSObj(currentItem());
			if (sobj == nullptr)
				return;

			sobj->Save(fileName.toStdString());
		});
	}

	mainMenu.exec(QCursor::pos());
}
